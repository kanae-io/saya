#ifndef SAYA_BRIDGE_HPP_
#define SAYA_BRIDGE_HPP_

#include "saya/program_options/utility.hpp"

#include "boost/optional.hpp"
//#include "boost/variant.hpp"
#include "boost/noncopyable.hpp"
#include "boost/program_options.hpp"
#include "boost/range/algorithm/find_if.hpp"

#include "boost/preprocessor/tuple.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "boost/preprocessor/if.hpp"
#include "boost/preprocessor/comparison.hpp"
#include "boost/preprocessor/expand.hpp"

#include <type_traits>
#include <functional>
#include <vector>

// positional option
#ifndef SAYA_BRIDGE_POSITIONAL_OPTION
# define SAYA_BRIDGE_POSITIONAL_OPTION ("input-file", -1)
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
#ifndef SAYA_BRIDGE_OPTION_GROUP_VARIABLE
# define SAYA_BRIDGE_OPTION_GROUP_VARIABLE option_groups
#endif
#ifndef SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE
# define SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE option_group_all
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
        option_group_type::iterator group = boost::find_if(this_->SAYA_BRIDGE_OPTION_GROUP_VARIABLE, [&group_id](option_group_element_type const& a) { return a.first == group_id; }); \
        \
        if (group == this_->SAYA_BRIDGE_OPTION_GROUP_VARIABLE.end()) { \
            this_->SAYA_BRIDGE_OPTION_GROUP_VARIABLE.emplace_back(group_id , std::string(SAYA_BRIDGE_DETAIL_OPTION_GROUP_DESCRIPTION)); \
            group = this_->SAYA_BRIDGE_OPTION_GROUP_VARIABLE.end() - 1; \
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
    static_assert( \
        std::is_same<decltype(this_->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name)), bool>::value, \
        "SAYA_BRIDGE_OPTION_EXISTENCE can only be used for bool members" \
    ); \
    if (vm.count(saya::program_options::to_option_id(option_name))) { \
        this_->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name)= true; \
    }

#define SAYA_BRIDGE_DETAIL_OPTION_FACTORY_ASSIGN_OPTIONAL(type, name, option_name) \
    this_->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = vm.count(saya::program_options::to_option_id(option_name)) ? \
        boost::make_optional(vm[saya::program_options::to_option_id(option_name)].as<type>()) : \
        boost::none;


#define SAYA_BRIDGE_DETAIL_OPTION_FACTORY(type, name, option_name, description, NEED_TYPE, ASSIGNER_TYPE) \
    struct BOOST_PP_CAT(SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name), _factory_t) : saya::detail::bridge_option_factory<bridge::self_type> \
    { \
        BOOST_PP_CAT(SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name), _factory_t) (self_type* this_) \
            : bridge_option_factory( \
                this_, \
                [](self_type* this_) { \
                    SAYA_BRIDGE_DETAIL_OPTION_FACTORY_INITIALIZE(type, name, option_name, description, NEED_TYPE) \
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

#define SAYA_BRIDGE_OPTION(type, name, option_name, description) \
    private: \
    type SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); \
    public: \
    type const& name() const { return this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); } \
    void name(type const& v) { this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = v; } \
    SAYA_BRIDGE_DETAIL_OPTION_FACTORY(type, name, option_name, description, 1, 0)

#define SAYA_BRIDGE_OPTION_DEFAULTED(type, name, default_value, option_name, description) \
    private: \
    type SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name){default_value}; \
    public: \
    type const& name() const { return this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); } \
    void name(type const& v) { this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = v; } \
    SAYA_BRIDGE_DETAIL_OPTION_FACTORY(type, name, option_name, description, 1, 0)

// optimize a little
#define SAYA_BRIDGE_OPTION_EXISTENCE(type, name, option_name, description) \
    private: \
    type SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); \
    public: \
    type name() const { return this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); } \
    void name(type v) { this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = v; } \
    SAYA_BRIDGE_DETAIL_OPTION_FACTORY(type, name, option_name, description, 0, 1)

#define SAYA_BRIDGE_OPTION_OPTIONAL(type, name) \
    private: \
    boost::optional<type> SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); \
    public: \
    bool BOOST_PP_CAT(has_, name) () const { return this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); } \
    type const& name() const { return *this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name); } \
    void name(::boost::optional<type> const& v_opt) { this->SAYA_BRIDGE_DETAIL_TO_VARIABLE_NAME(name) = v_opt; } \
    SAYA_BRIDGE_DETAIL_OPTION_FACTORY(type, name, option_name, description, 1, 2)


namespace saya {

struct option_error : std::logic_error
{
    using std::logic_error::logic_error;
};

namespace detail {

template<class Self>
struct bridge_option_factory
{
    typedef Self self_type;

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
    typedef Self self_type;
    typedef boost::program_options::variables_map vm_type;
    typedef std::function<void ()> initializer_type;
    typedef std::function<void (vm_type const&)> assigner_type;

    virtual ~bridge() = default;

    virtual void init(int const argc, char const* const argv[]) try
    {
        // don't put this in constructor,
        // since this must be called after all constructors
        for (auto const& initializer : initializers_) {
            initializer();
        }
        this->finalize_option_groups();

        // vm
        boost::program_options::variables_map vm;

        // store
        boost::program_options::store(
            boost::program_options::command_line_parser(argc, argv)
                .options(this->SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE)
                .positional(this->SAYA_BRIDGE_POSITIONAL_OPTION_VARIABLE)
                .run(),
            vm
        );

        // assign, validate, notify
        for (auto const& assigner : assigners_) {
            assigner(vm);
        }
        this->validate(vm);
        boost::program_options::notify(vm);

    } catch (boost::program_options::invalid_option_value const& e) {
        throw option_error(e.what());
    }
    virtual void validate(vm_type const& vm) = 0;

    void add_initializer(initializer_type f) { initializers_.push_back(f); }
    void add_assigner(assigner_type f) { assigners_.push_back(f); }

    // variables
    typedef std::pair<std::string, boost::program_options::options_description> option_group_element_type;
    typedef std::vector<option_group_element_type> option_group_type;
    option_group_type SAYA_BRIDGE_OPTION_GROUP_VARIABLE;
    boost::program_options::options_description SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE;
    boost::program_options::positional_options_description SAYA_BRIDGE_POSITIONAL_OPTION_VARIABLE;

protected:
    bridge()
    {
        this->option_positional.add(BOOST_PP_TUPLE_ENUM(2, SAYA_BRIDGE_POSITIONAL_OPTION));
    }

private:
    bridge(bridge const&) = delete;
    bridge& operator=(bridge const&) = delete;

    void finalize_option_groups()
    {
        for (auto const& group_kv : SAYA_BRIDGE_OPTION_GROUP_VARIABLE) {
            this->SAYA_BRIDGE_OPTION_GROUP_ALL_VARIABLE.add(group_kv.second);
        }
    }


    std::vector<initializer_type> initializers_;
    std::vector<assigner_type> assigners_;
};

} // saya

#endif
