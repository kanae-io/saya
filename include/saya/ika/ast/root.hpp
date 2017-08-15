#ifndef SAYA_IKA_AST_ROOT_HPP
#define SAYA_IKA_AST_ROOT_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/detail/lookup_query.hpp"
#include "saya/ika/ast/namespace.hpp"

#include "saya/logger.hpp"

#include <boost/range/adaptor/transformed.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include <boost/assert.hpp>

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <deque>
#include <memory>
#include <stdexcept>


namespace saya { namespace ika { namespace ast {

namespace detail {

template<class T, class Enable = void>
struct proxy_dispatcher;

} // detail

class Root //: ASTEntity
{
    template<class T>
    static constexpr bool is_block_v = std::is_same<Geo, T>::value || std::is_same<Block, T>::value;

    template<class T>
    static constexpr bool is_literal_v = std::is_base_of<LiteralEntity, T>::value;

public:
    enum class Context
    {
        NAMESPACE,
    };

    template<class T, class Enable>
    friend struct detail::proxy_dispatcher;

    Root()
    {
        auto global_ns = std::make_unique<Namespace>(NSID{Namespace::GLOBAL_ID()});
        detail::LookupQuery<Namespace> query{global_ns->id};
        query.ctx_hash = boost::hash<NSID>()(global_ns->id);

        namespaces_[query] = std::move(global_ns);
        namespace_stack_.init(query.ctx_hash);
    }

    Root(Root const&) = delete;
    Root(Root&&) = delete;

    ~Root() = default;

    Root& operator=(Root const&) = delete;
    Root& operator=(Root&&) = delete;

    std::vector<Stmt> stmt_list;

    // -------------------------------------------------------

    // context pop
    void operator[](Context ctx)
    {
        switch (ctx) {
        case Context::NAMESPACE:
            namespace_stack_.pop();
            break;
        }
    }

    // -------------------------------------------------------

    template<
        class T,
        typename std::enable_if_t<
            is_block_v<T>,
            int
        > = 0
    >
    T* operator[](std::unique_ptr<T> p)
    {
        auto& m = this->*(detail::proxy_dispatcher<T>::dispatch());
        m.emplace_back(std::move(p));
        return m.back().get();
    }

    template<
        class T,
        typename std::enable_if_t<
            std::is_same<Namespace, T>::value,
            int
        > = 0
    >
    T* operator[](detail::LookupQuery<T> query)
    {
        namespace_stack_.make_fqns(query.qualifier);
        query.ctx_hash = namespace_stack_.hash_query(query, true);
        auto& m = this->*(detail::proxy_dispatcher<T>::dispatch());

        if (!m.count(query)) {
            m[query] = std::make_unique<T>(
                NSID{namespace_stack_.fqns() + Namespace::SEP() + flyweights::extractor{}(query.target)}
            );
        }
        namespace_stack_.push(query);
        return m.at(query).get();
    }

    template<
        class T,
        typename std::enable_if_t<
            std::is_same<Var, T>::value ||
            std::is_same<Func, T>::value ||
            std::is_same<Macro, T>::value,
            int
        > = 0
    >
    T* operator[](detail::LookupQuery<T> query)
    {
        namespace_stack_.make_fqns(query.qualifier);
        query.ctx_hash = namespace_stack_.hash_query(query, true);
        auto& m = this->*(detail::proxy_dispatcher<T>::dispatch());

        if (m.count(query)) {
            #if !defined(NDEBUG)
            ++prof_id_.hit;
            #endif

        } else {
            #if !defined(NDEBUG)
            ++prof_id_.miss;
            #endif

            m[query] = std::make_unique<T>(
                decltype(query.target){
                    namespace_stack_.fqns() + Namespace::SEP() + flyweights::extractor{}(query.target)
                }
            );
        }

        return m.at(query).get();
    }

    template<
        class T,
        typename std::enable_if_t<
            std::is_same<Group, T>::value ||
            std::is_same<Endpoint, T>::value,
            int
        > = 0
    >
    T* operator[](detail::LookupQuery<T> query)
    {
        // namespace_stack_.make_fqns(query.qualifier);
        query.ctx_hash = namespace_stack_.hash_query(query, false);
        auto& m = this->*(detail::proxy_dispatcher<T>::dispatch());

        if (m.count(query)) {
           // ...

        } else {
            m[query] = std::make_unique<T>(
                decltype(query.target){
                    boost::algorithm::join(
                        query.qualifier | boost::adaptors::transformed(flyweights::extractor{}),
                        "."
                    ) + "." + flyweights::extractor{}(query.target)
                }
            );
        }

        return m.at(query).get();
    }

    template<
        class T,
        typename std::enable_if_t<!is_block_v<T> && is_literal_v<T>, int> = 0
    >
    T* operator[](std::unique_ptr<T> p)
    {
        BOOST_ASSERT(p);

        auto& m = this->*(detail::proxy_dispatcher<T>::dispatch());
        auto const* const pp = p.get();
        auto it = m.find(pp);

        if (it != m.end()) {
            return static_cast<T*>(it->second.get());

        } else {
            auto const inserted = m.insert(std::make_pair(pp, std::move(p)));

            BOOST_ASSERT(inserted.second);
            BOOST_ASSERT(inserted.first->second.get() == pp);
            return static_cast<T*>(inserted.first->second.get());
        }
    }

    #if !defined(NDEBUG)
    template<class Stream>
    inline void debug(Stream&& os) const
    {
        os << boost::format("Identifier cache hit rate: %.2f%%") % prof_id_.rate() << "\n";
    }
    #else
    template<class Stream>
    inline void debug(Stream&&) const {}
    #endif

private:
    // ------------------------------------------------------
    using literals_type = std::unordered_map<
        LiteralEntity const*,
        std::unique_ptr<LiteralEntity>,
        lit::deep_hash<LiteralEntity>
    >;
    literals_type literals_;

    // ------------------------------------------------------

    using geos_type = std::vector<std::unique_ptr<Geo>>;
    geos_type geos_;

    using blocks_type = std::vector<std::unique_ptr<Block>>;
    blocks_type blocks_;

    // ------------------------------------------------------

    using namespaces_type = std::unordered_map<
        detail::LookupQuery<Namespace>, std::unique_ptr<Namespace>,
        boost::hash<detail::LookupQuery<Namespace>>
    >;
    namespaces_type namespaces_;

    class NamespaceStack
    {
    public:
        void init(std::size_t global_ctx)
        {
            BOOST_ASSERT(this->empty());
            ctx_.push_front(global_ctx);
            fqns_queue_.push_back(NSID{Namespace::GLOBAL_ID()});
        }

        void push(detail::LookupQuery<Namespace> const& query)
        {
            ctx_.push_front(query.ctx_hash);
            fqns_queue_.push_back(query.target);
        }

        void pop()
        {
            BOOST_ASSERT(!this->empty());
            ctx_.pop_front();
            fqns_queue_.pop_back();
        }

        template<class T>
        inline std::size_t hash_query(detail::LookupQuery<T> const& query, bool include_ns) const
        {
            BOOST_ASSERT(!this->empty());

            std::size_t seed = include_ns ? ctx_.front() : 0;
            boost::hash_combine(seed, boost::hash_range(query.qualifier.begin(), query.qualifier.end()));
            boost::hash_combine(seed, query.target);
            return seed;
        }

        inline std::string fqns() const
        {
            return boost::algorithm::join(
                fqns_queue_ | boost::adaptors::transformed(
                    flyweights::extractor{}
                ),
                Namespace::SEP()
            );
        }

        inline void
        make_fqns(std::deque<NSID>& child_qualifier) const
        {
            std::copy(fqns_queue_.begin(), fqns_queue_.end(), std::front_inserter(child_qualifier));
        }

        inline bool empty() const noexcept { return ctx_.empty(); }

    private:
        std::deque<std::size_t> ctx_;
        std::deque<NSID> fqns_queue_;
    };
    NamespaceStack namespace_stack_;

    // ------------------------------------------------------

    using vars_type = std::unordered_map<
        detail::LookupQuery<Var>, std::unique_ptr<Var>,
        boost::hash<detail::LookupQuery<Var>>
    >;
    vars_type vars_;

    using funcs_type = std::unordered_map<
        detail::LookupQuery<Func>, std::unique_ptr<Func>,
        boost::hash<detail::LookupQuery<Func>>
    >;
    funcs_type funcs_;

    using macros_type = std::unordered_map<
        detail::LookupQuery<Macro>, std::unique_ptr<Macro>,
        boost::hash<detail::LookupQuery<Macro>>
    >;
    macros_type macros_;

    // ------------------------------------------------------

    using groups_type = std::unordered_map<
        detail::LookupQuery<Group>, std::unique_ptr<Group>,
        boost::hash<detail::LookupQuery<Group>>
    >;
    groups_type groups_;

    using endpoints_type = std::unordered_map<
        detail::LookupQuery<Endpoint>, std::unique_ptr<Endpoint>,
        boost::hash<detail::LookupQuery<Endpoint>>
    >;
    endpoints_type endpoints_;


    #if !defined(NDEBUG)
    struct CacheProf
    {
        unsigned miss{0}, hit{0};
        double rate() const noexcept { return double(hit) / (miss + hit) * 100; }
    };
    CacheProf prof_id_;
    #endif
};

namespace detail {

template<>
struct proxy_dispatcher<Namespace>
{
    static auto dispatch() noexcept
        -> decltype(&Root::namespaces_)
    {
        return &Root::namespaces_;
    }
};

template<>
struct proxy_dispatcher<Geo>
{
    static auto dispatch() noexcept
        -> decltype(&Root::geos_)
    {
        return &Root::geos_;
    }
};
template<>
struct proxy_dispatcher<Block>
{
    static auto dispatch() noexcept
        -> decltype(&Root::blocks_)
    {
        return &Root::blocks_;
    }
};

template<class T>
struct proxy_dispatcher<T, typename std::enable_if_t<std::is_base_of<LiteralEntity, T>::value>>
{
    static auto dispatch() noexcept
        -> decltype(&Root::literals_)
    {
        return &Root::literals_;
    }
};

template<>
struct proxy_dispatcher<Group>
{
    static auto dispatch() noexcept
        -> decltype(&Root::groups_)
    {
        return &Root::groups_;
    }
};
template<>
struct proxy_dispatcher<Endpoint>
{
    static auto dispatch() noexcept
        -> decltype(&Root::endpoints_)
    {
        return &Root::endpoints_;
    }
};
template<>
struct proxy_dispatcher<Var>
{
    static auto dispatch() noexcept
        -> decltype(&Root::vars_)
    {
        return &Root::vars_;
    }
};
template<>
struct proxy_dispatcher<Func>
{
    static auto dispatch() noexcept
        -> decltype(&Root::funcs_)
    {
        return &Root::funcs_;
    }
};
template<>
struct proxy_dispatcher<Macro>
{
    static auto dispatch() noexcept
        -> decltype(&Root::macros_)
    {
        return &Root::macros_;
    }
};

} // detail

}}} // saya

#endif
