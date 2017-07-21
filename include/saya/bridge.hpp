#ifndef SAYA_BRIDGE_HPP
#define SAYA_BRIDGE_HPP

#include "saya/program_options/utility.hpp"

#include <boost/optional.hpp>
//#include <boost/variant.hpp>
#include <boost/noncopyable.hpp>
#include <boost/program_options.hpp>
#include <boost/range/algorithm/find_if.hpp>

#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/comparison.hpp>
#include <boost/preprocessor/expand.hpp>

#include <type_traits>
#include <functional>
#include <vector>

// positional option
#define SAYA_BRIDGE_DETAIL_TO_MEMBER(var) BOOST_PP_CAT(var, _)

#ifndef SAYA_BRIDGE_POSITIONAL_OPTION
# define SAYA_BRIDGE_POSITIONAL_OPTION ("input-files", -1)
#endif

#define SAYA_BRIDGE_POSITIONAL_OPTION_ID \
    BOOST_PP_TUPLE_ELEM(2, 0, SAYA_BRIDGE_POSITIONAL_OPTION)

#define SAYA_BRIDGE_POSITIONAL_OPTION_COUNT \
    BOOST_PP_TUPLE_ELEM(2, 1, SAYA_BRIDGE_POSITIONAL_OPTION)


// option group
#define SAYA_BRIDGE_DETAIL_OPTION_GROUP_ID \
    BOOST_PP_TUPLE_ELEM(2, 0, SAYA_BRIDGE_OPTION_GROUP)

#define SAYA_BRIDGE_DETAIL_OPTION_GROUP_DESCRIPTION \
    BOOST_PP_TUPLE_ELEM(2, 1, SAYA_BRIDGE_OPTION_GROUP)

// configurable names
#ifndef SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE
# define SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE option_holder_all
#endif
#ifndef SAYA_BRIDGE_OPTION_HOLDER_VISIBLE_VARIABLE
# define SAYA_BRIDGE_OPTION_HOLDER_VISIBLE_VARIABLE option_holder_visible
#endif
#ifndef SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE
# define SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE option_group_all
#endif
#ifndef SAYA_BRIDGE_OPTION_GROUP_VISIBLE_VARIABLE
# define SAYA_BRIDGE_OPTION_GROUP_VISIBLE_VARIABLE option_group_visible
#endif
#ifndef SAYA_BRIDGE_POSITIONAL_OPTION_VARIABLE
# define SAYA_BRIDGE_POSITIONAL_OPTION_VARIABLE option_positional
#endif


// variable name helper
#define SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) BOOST_PP_CAT(name, _)


// initializer
#define SAYA_PO_TYPE(type, NEED_TYPE) \
    BOOST_PP_IF( \
        NEED_TYPE, \
        ::boost::program_options::value<type>(), \
        BOOST_PP_EMPTY() \
    )

#define SAYA_BRIDGE_DETAIL_OPTION_FACTORY_INITIALIZE(type, name, option_name, description, NEED_TYPE) \
    { \
        std::string const& group_id = BOOST_PP_STRINGIZE(SAYA_BRIDGE_DETAIL_OPTION_GROUP_ID); \
        option_holder_type::iterator group = boost::find_if(this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE), [&group_id](option_group_element_type const& a) { return a.first == group_id; }); \
        \
        if (group == this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE).end()) { \
            this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE).emplace_back(group_id , std::string(SAYA_BRIDGE_DETAIL_OPTION_GROUP_DESCRIPTION)); \
            group = this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE).end() - 1; \
        } \
        group->second.add_options()( \
            option_name, \
            SAYA_PO_TYPE(type, NEED_TYPE) BOOST_PP_COMMA_IF(NEED_TYPE) \
            description \
        ); \
        group = boost::find_if(this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_VISIBLE_VARIABLE), [&group_id](option_group_element_type const& a) { return a.first == group_id; }); \
        \
        if (group == this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_VISIBLE_VARIABLE).end()) { \
            this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_VISIBLE_VARIABLE).emplace_back(group_id , std::string(SAYA_BRIDGE_DETAIL_OPTION_GROUP_DESCRIPTION)); \
            group = this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_VISIBLE_VARIABLE).end() - 1; \
        } \
        group->second.add_options()( \
            option_name, \
            SAYA_PO_TYPE(type, NEED_TYPE) BOOST_PP_COMMA_IF(NEED_TYPE) \
            description \
        ); \
    }

#define SAYA_BRIDGE_DETAIL_OPTION_FACTORY_INITIALIZE_POSITIONAL(type, name, option_name, description, NEED_TYPE) \
    { \
        std::string const& group_id = BOOST_PP_STRINGIZE(SAYA_BRIDGE_DETAIL_OPTION_GROUP_ID); \
        option_holder_type::iterator group = boost::find_if(this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE), [&group_id](option_group_element_type const& a) { return a.first == group_id; }); \
        \
        if (group == this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE).end()) { \
            this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE).emplace_back(group_id , std::string(SAYA_BRIDGE_DETAIL_OPTION_GROUP_DESCRIPTION)); \
            group = this_->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE).end() - 1; \
        } \
        group->second.add_options()( \
            option_name, \
            SAYA_PO_TYPE(type, NEED_TYPE) BOOST_PP_COMMA_IF(NEED_TYPE) \
            description \
        ); \
    }

// assigners
#define SAYA_BRIDGE_DETAIL_OPTION_FACTORY_ASSIGN(type, name, option_name) \
    if (vm.count(saya::program_options::to_option_id(option_name))) { \
        this_->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = vm[saya::program_options::to_option_id(option_name)].as<type>(); \
    }

#define SAYA_BRIDGE_DETAIL_OPTION_FACTORY_ASSIGN_EXISTENCE(type, name, option_name) \
    if (vm.count(saya::program_options::to_option_id(option_name))) { \
        this_->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name)= true; \
    }

#define SAYA_BRIDGE_DETAIL_OPTION_FACTORY_ASSIGN_OPTIONAL(type, name, option_name) \
    this_->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = vm.count(saya::program_options::to_option_id(option_name)) ? \
        boost::make_optional(vm[saya::program_options::to_option_id(option_name)].as<type>()) : \
        boost::none;


#define SAYA_BRIDGE_DETAIL_OPTION_FACTORY(type, name, option_name, description, NEED_TYPE, ASSIGNER_TYPE, INITIALIZER_MACRO) \
    struct BOOST_PP_CAT(SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name), _factory_t) : saya::detail::bridge_option_factory<bridge::self_type> \
    { \
        BOOST_PP_CAT(SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name), _factory_t) (self_type* this_) \
            : bridge_option_factory( \
                this_, \
                [](self_type* this_) { \
                    INITIALIZER_MACRO(type, name, option_name, description, NEED_TYPE) \
                }, \
                [](self_type* this_, boost::program_options::variables_map const& vm) { \
                    BOOST_PP_IF( \
                        BOOST_PP_EQUAL(ASSIGNER_TYPE, 0), \
                        SAYA_BRIDGE_DETAIL_OPTION_FACTORY_ASSIGN(type, name, option_name), \
                        \
                        BOOST_PP_IF( \
                            BOOST_PP_EQUAL(ASSIGNER_TYPE, 1), \
                            SAYA_BRIDGE_DETAIL_OPTION_FACTORY_ASSIGN_EXISTENCE(type, name, option_name), \
                            \
                            BOOST_PP_IF( \
                                BOOST_PP_EQUAL(ASSIGNER_TYPE, 2), \
                                SAYA_BRIDGE_DETAIL_OPTION_FACTORY_ASSIGN_OPTIONAL(type, name, option_name), \
                                static_assert(false, ""); \
                            ) \
                        ) \
                    ) \
                } \
             ) \
        {} \
    } BOOST_PP_CAT(SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name), _initializer_t_){this};


#define SAYA_BRIDGE_DETAIL_OPTION_IMPL(type, name, option_name, description, INITIALIZER_MACRO) \
    private: \
    type SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); \
    public: \
    type const& name() const noexcept { return this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); } \
    void name(type const& v) { this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = v; } \
    SAYA_BRIDGE_DETAIL_OPTION_FACTORY(type, name, option_name, description, 1, 0, INITIALIZER_MACRO)

#define SAYA_BRIDGE_OPTION(type, name, option_name, description) \
    SAYA_BRIDGE_DETAIL_OPTION_IMPL(type, name, option_name, description, SAYA_BRIDGE_DETAIL_OPTION_FACTORY_INITIALIZE)

#define SAYA_BRIDGE_OPTION_POSITIONAL(type, name, option_name, description) \
    SAYA_BRIDGE_DETAIL_OPTION_IMPL(type, name, option_name, description, SAYA_BRIDGE_DETAIL_OPTION_FACTORY_INITIALIZE_POSITIONAL)

#define SAYA_BRIDGE_OPTION_DEFAULTED(type, name, default_value, option_name, description) \
    private: \
    type SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name){default_value}; \
    public: \
    type const& name() const noexcept { return this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); } \
    void name(type const& v) { this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = v; } \
    SAYA_BRIDGE_DETAIL_OPTION_FACTORY(type, name, option_name, description, 1, 0, SAYA_BRIDGE_DETAIL_OPTION_FACTORY_INITIALIZE)

// optimize a little
#define SAYA_BRIDGE_OPTION_EXISTENCE(name, option_name, description) \
    private: \
    bool SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name){false}; \
    public: \
    bool name() const noexcept { return this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); } \
    void name(bool v) { this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = v; } \
    SAYA_BRIDGE_DETAIL_OPTION_FACTORY(bool, name, option_name, description, 0, 1, SAYA_BRIDGE_DETAIL_OPTION_FACTORY_INITIALIZE)

#define SAYA_BRIDGE_OPTION_OPTIONAL(type, name) \
    private: \
    boost::optional<type> SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); \
    public: \
    bool BOOST_PP_CAT(has_, name) () const { return this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); } \
    type const& name() const noexcept { return *this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); } \
    void name(::boost::optional<type> const& v_opt) { this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = v_opt; } \
    SAYA_BRIDGE_DETAIL_OPTION_FACTORY(type, name, option_name, description, 1, 2, SAYA_BRIDGE_DETAIL_OPTION_FACTORY_INITIALIZE)


namespace saya {

struct validation_error : std::invalid_argument
{
    using std::invalid_argument::invalid_argument;
};

namespace detail {

template<class Self>
struct bridge_option_factory
{
    using self_type = Self;

    template<class Initializer, class Assigner>
    bridge_option_factory(self_type* this_, Initializer initializer, Assigner assigner)
    {
        this_->add_initializer(std::bind(initializer, this_));
        this_->add_assigner(std::bind(assigner, this_, std::placeholders::_1));
    }
};
} // detail

template<class Self>
class bridge
{
public:
    using base_type = bridge<Self>;
    using self_type = Self;
    using vm_type = boost::program_options::variables_map;
    using initializer_type = std::function<void ()>;
    using assigner_type = std::function<void (vm_type const&)>;

    virtual ~bridge() = default;

    void init(int const argc, char const* const argv[]) try
    {
        // don't put this in constructor,
        // this must be called after all constructors
        for (auto const& initializer : initializers_) {
            initializer();
        }
        this->finalize_option_groups();

        // vm
        boost::program_options::variables_map vm;

        // store
        boost::program_options::store(
            boost::program_options::command_line_parser(argc, argv)
                .options(this->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE))
                .positional(this->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_POSITIONAL_OPTION_VARIABLE))
                .run(),
            vm
        );

        // assign, validate, notify
        for (auto const& assigner : assigners_) {
            assigner(vm);
        }
        this->validate(vm);
        boost::program_options::notify(vm);

    } catch (boost::program_options::error const& e) {
        throw validation_error(e.what());
    }


    void add_initializer(initializer_type f) { initializers_.push_back(f); }
    void add_assigner(assigner_type f) { assigners_.push_back(f); }

    // accessors
    auto const& SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE () const noexcept
    {
        return this->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE);
    }
    auto const& SAYA_BRIDGE_OPTION_GROUP_VISIBLE_VARIABLE () const noexcept
    {
        return this->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_GROUP_VISIBLE_VARIABLE);
    }
    auto const& SAYA_BRIDGE_POSITIONAL_OPTION_VARIABLE () const noexcept
    {
        return this->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_POSITIONAL_OPTION_VARIABLE);
    }

protected:
    using option_group_element_type = std::pair<std::string, boost::program_options::options_description>;

    boost::program_options::options_description
    SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE);

    boost::program_options::options_description
    SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_GROUP_VISIBLE_VARIABLE);

    boost::program_options::positional_options_description
    SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_POSITIONAL_OPTION_VARIABLE);

    using option_holder_type = std::vector<option_group_element_type>;

    option_holder_type SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE);
    option_holder_type SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_VISIBLE_VARIABLE);

    virtual void validate(vm_type const& vm) = 0;

    bridge()
    {
        this->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_POSITIONAL_OPTION_VARIABLE).add(BOOST_PP_TUPLE_ENUM(2, SAYA_BRIDGE_POSITIONAL_OPTION));
    }

private:
    bridge(bridge const&) = delete;
    bridge& operator=(bridge const&) = delete;

    void finalize_option_groups()
    {
        for (auto const& group_kv : SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_ALL_VARIABLE)) {
            this->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE).add(group_kv.second);
        }
        for (auto const& group_kv : SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_HOLDER_VISIBLE_VARIABLE)) {
            this->SAYA_BRIDGE_DETAIL_TO_MEMBER(SAYA_BRIDGE_OPTION_GROUP_VISIBLE_VARIABLE).add(group_kv.second);
        }
    }


    std::vector<initializer_type> initializers_;
    std::vector<assigner_type> assigners_;
};

} // saya

#endif
