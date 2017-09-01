#include "saya/zed/core.hpp"
#include "saya/zed/memory.hpp"
#include "saya/zed/debugger.hpp"
#include "saya/zed/terminal.hpp"

#include "saya/ika/ast/fusion_adapt.hpp"
#include "saya/ika/grammar/to_ast.hpp"
#include "saya/ika/grammar/detail/spirit_attribute_support.hpp"

// qi basic
#include <boost/spirit/include/qi_domain.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp> // important stuff & on_error
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_attr.hpp>
#include <boost/spirit/include/qi_attr_cast.hpp>

// ns / char
#include <boost/spirit/include/qi_as_string.hpp>

// parser transformers
#include <boost/spirit/include/qi_optional.hpp>

// trivial parsers
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_eoi.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_string.hpp>

// parser modifiers, etc.
#include <boost/spirit/include/qi_raw.hpp>
#include <boost/spirit/include/qi_as.hpp>
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/qi_skip.hpp>
#include <boost/spirit/include/qi_no_skip.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_hold.hpp>

// special parsers
#include <boost/spirit/include/qi_list.hpp>
#include <boost/spirit/include/qi_eps.hpp>

// attrs, locals, extra support
#include <boost/spirit/include/support.hpp> // _val, sl::_1, _pass, etc.

// fusion
#include <boost/fusion/include/std_pair.hpp>

// phoenix
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/core/actor.hpp> // val
#include <boost/phoenix/core/value.hpp> // val
#include <boost/phoenix/core/reference.hpp> // ref
#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/object/static_cast.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/phoenix/fusion/at.hpp>
#include <boost/phoenix/stl/container.hpp>

#include <boost/core/ignore_unused.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <iostream>
#include <memory>
#include <cstdint>


namespace saya { namespace ika { namespace grammar {

#if 0
#define SAYA_IKA_DESTRUCTOR_REGISTER(func) \
    (qi::eps [phx::bind(&::saya::zed::destructor_register, sl::_a, (func))])

#define SAYA_IKA_DETAIL_DESTRUCTOR_CALL() \
    (qi::eps [phx::bind(&::saya::zed::destructor_call, sl::_a)])

#define SAYA_IKA_DESTRUCTOR_SCOPE(parser) \
    (qi::eps [ \
        sl::_a = phx::construct<::saya::zed::destructor_handler>() \
    ] >> \
    ( \
        ((parser) >> (SAYA_IKA_DETAIL_DESTRUCTOR_CALL())) | \
        ((SAYA_IKA_DETAIL_DESTRUCTOR_CALL()) >> qi::eps [sp::_pass = phx::val(false)]) \
    ))
#endif

namespace {

// ------------------------------------------------------
// original parsers
zed::terminals::hex_of_t<typename ast::lit::Color::internal_type, 6> const
hex_of_rrggbb_;

zed::terminals::hex_of_t<typename ast::lit::Color::internal_type, 3> const
hex_of_rgb_;

} // anon


class to_ast::Impl
{
public:
    explicit Impl(to_ast::root_rule_type& root_)
    {
        namespace sp = boost::spirit;
        namespace sl = boost::spirit::labels;
        namespace qi = boost::spirit::qi;
        namespace ns = boost::spirit::qi::standard;
        namespace zt = saya::zed::terminals;
        namespace phx = boost::phoenix;

        // ---------------------------------------
        root_.name("root");
        root_ =
            qi::eps [sl::_val = zed::make_unique<ast::Root>()] >
            (
                (!qi::eoi >> stmt_(phx::bind(&to_ast::root_type::get, sl::_val)) [phx::push_back(phx::at_c<0>(*sl::_val), sl::_1)]) % stmt_break_
            ) >
            +qi::eol > qi::eoi
        ;
        debug_(root_);

        stmt_.name("stmt");
        stmt_ =
            namespace_(sl::_r1) |

            geo_(sl::_r1) |

            default_specifier_(sl::_r1) |

            (qi::lit("def") > (group_definition_(sl::_r1) | func_definition_(sl::_r1) | macro_definition_(sl::_r1))) |

            declaration_(sl::_r1) |

            se_expr_(sl::_r1) |
            empty_stmt_
        ;
        debug_(stmt_);

        empty_stmt_.name("empty-stmt");
        empty_stmt_ =
            qi::lit(';')
        ;
        debug_(empty_stmt_);

        stmt_break_.name("stmt-break");
        stmt_break_ = +(qi::eol | qi::lit(';'));
        debug_(stmt_break_);

        primary_expr_.name("primary-expr");
        primary_expr_ = qi::skip(ns::blank) [
            // non-trivial ones
            lit_Map_(sl::_r1) |

            lit_Pct_(sl::_r1) | lit_Px_(sl::_r1) |  // uint + suffix

            // trivial ones, but lazy
            lit_String_(sl::_r1) | lit_Color_(sl::_r1) |

            lit_Real_(sl::_r1) | /* with DOT */
            lit_Int64_(sl::_r1) | /* negative, or expicitly-specified */

            // UInt64 is always the last,
            // because it has no characters other than [0-9]
            lit_UInt64_(sl::_r1) |

            // Literals are always evaluated earlier,
            // because there shall be more Literal than others
            // in this Ika world.
            var_(sl::_r1) |

            (qi::lit('(') > qi::skip(ns::space) [expr_(sl::_r1)] > qi::lit(')'))
        ];
        debug_(primary_expr_);

        se_expr_.name("side-effect-expr");
        se_expr_ = qi::skip [
            qi::skip(ns::blank) [uop_macro_call_(sl::_r1)] |
            bop_assign_(sl::_r1)
        ];
        debug_(se_expr_);

        // see: http://en.cppreference.com/w/cpp/language/operator_precedence
        expr_.name("expr");
        expr_ = qi::skip [
            qi::skip(ns::blank) [
                uop_func_call_(sl::_r1)
            ] |

            uop_add_family_(sl::_r1) |

            bop_mul_family_(sl::_r1) | // bop_pow_(sl::_r1) |

            // bop_add_family_(sl::_r1) |

            // bop_assign_(sl::_r1)

            primary_expr_(sl::_r1)
        ];
        debug_(expr_);

        declaration_.name("declaration");
        declaration_ =
            qi::lexeme [
                qi::omit [-qi::lit('!') [phx::at_c<2>(sl::_val) = false]] >>
                (
                    group_(sl::_r1, false) |
                    endpoint_(sl::_r1)
                ) [phx::at_c<0>(sl::_val) = sl::_1]
            ] >
            -attribute_(sl::_r1) [phx::at_c<1>(sl::_val) = sl::_1]
        ;
        debug_(declaration_);

        func_definition_.name("func-definition");
        func_definition_ = (
            func_(sl::_r1) [phx::at_c<0>(sl::_val) = sl::_1] >
            block_(sl::_r1) [phx::at_c<1>(sl::_val) = sl::_1]
        ) [(*sl::_1)[sl::_2]];
        debug_(func_definition_);

        macro_definition_.name("macro-definition");
        macro_definition_ = (
            macro_(sl::_r1) [phx::at_c<0>(sl::_val) = sl::_1] >
            geo_(sl::_r1) [phx::at_c<1>(sl::_val) = sl::_1]
        ) [(*sl::_1)[sl::_2]];
        debug_(macro_definition_);

        group_child_specifier_.name("group-child-specifier");
        group_child_specifier_ =
            qi::lit('[') >
            qi::skip(ns::space) [qi::eps] >
            qi::no_skip [
                group_(sl::_r1, false) [phx::at_c<0>(sl::_val) = sl::_1]
            ] >
            qi::skip(ns::blank) [
                -attribute_(sl::_r1) [phx::at_c<1>(sl::_val) = sl::_1]
            ] >
            qi::skip(ns::space) [qi::eps] >
            qi::lit(']')
        ;
        debug_(group_child_specifier_);

        group_definition_.name("group-definition");
        group_definition_ = (
            qi::lexeme [
                group_(sl::_r1, true) [phx::at_c<0>(sl::_val) = sl::_1]
            ] >
            qi::no_skip [
                -group_child_specifier_(sl::_r1) [phx::at_c<1>(sl::_val) = sl::_1]
            ] >
            geo_(sl::_r1) [phx::at_c<2>(sl::_val) = sl::_1] >
            qi::eps [(*sl::_r1)[ast::Root::Context::GROUP]]
        ); // [(*sl::_1)[sl::_3]];
        debug_(group_definition_);

        additional_class_.name("additional-class");
        additional_class_ = (
            *qi::as_string [qi::lit('.') > raw_group_id_]
        ) [sl::_val = (*sl::_r1)[zed::make_unique<ast::AdditionalClass>(sl::_1)]];
        debug_(additional_class_);

        attribute_.name("attribute");
        attribute_ = (
            qi::lit('[') >
            (
                qi::eps [sl::_a = phx::construct<std::vector<ast::Attribute::attr_type>>()] >
                (
                    (
                        (!qi::lit(']')) > (
                            expr_(sl::_r1) [phx::push_back(sl::_a, sl::_1)] |
                            placeholdable_(sl::_r1) [phx::push_back(sl::_a, sl::_1)]
                        )
                    )
                    % qi::lit(',')
                )
            ) >
            qi::lit(']')
        ) [sl::_val = (*sl::_r1)[zed::make_unique<ast::Attribute>(sl::_a)]];
        debug_(attribute_);

        // -------------------------------------------------------
        // expr meta
        u_entity_.name("unary-entity");
        u_entity_ =
            qi::skip(ns::blank) [
                uop_func_call_(sl::_r1) |
                uop_subscript_(sl::_r1) |
                uop_add_family_(sl::_r1)
            ] |

            qi::skip [uop_not_(sl::_r1)] |

            qi::skip [primary_expr_(sl::_r1)]
        ;
        debug_(u_entity_);

        // -------------------------------------------------------
        // uop
        uop_func_call_.name("uop-func-call");
        uop_func_call_ =
            qi::lexeme [func_(sl::_r1)] >
            qi::skip(ns::blank) [call_param_(sl::_r1)] >
            qi::skip(ns::blank) [-block_(sl::_r1)]
        ;
        debug_(uop_func_call_);

        uop_macro_call_.name("uop-macro-call");
        uop_macro_call_ =
            qi::lexeme [macro_(sl::_r1)] >
            qi::no_skip [-additional_class_(sl::_r1)] >
            qi::skip(ns::blank) [-call_param_(sl::_r1)] >
            qi::skip(ns::blank) [-geo_(sl::_r1)]
        ;
        debug_(uop_macro_call_);

        uop_not_.name("uop-not");
        uop_not_ =
            qi::lit('!') > primary_expr_(sl::_r1)
        ;
        debug_(uop_not_);

        uop_add_family_.name("uop-add-family");
        uop_add_family_ = qi::eps >> (
            (qi::lit('-') > qi::attr(false) > primary_expr_(sl::_r1)) |
            (qi::lit('+') > qi::attr(true) > primary_expr_(sl::_r1))
        );
        debug_(uop_add_family_);

        uop_subscript_.name("uop-subscript");
        uop_subscript_ =
            var_(sl::_r1) >> (
                qi::lit('[') >
                qi::lexeme [
                    qi::lit(ast::lit::Symbol::MAGIC_TOKEN()) > lit_Symbol_(sl::_r1)
                ] >
                qi::lit(']')
            )
        ;
        debug_(uop_subscript_);

        // -------------------------------------------------------
        // bop
        bop_mul_family_.name("bop-mul-family");
        bop_mul_family_ = qi::skip [
            u_entity_(sl::_r1) >>
            +(
                (
                    qi::eps >>
                    ((qi::lit('*') > qi::attr(true)) | qi::lit('/') > qi::attr(false))
                ) >
                u_entity_(sl::_r1)
            )
        ];
        debug_(bop_mul_family_);

        bop_add_family_.name("bop-add-family");
        bop_add_family_ = qi::skip [
            u_entity_(sl::_r1) >>
            +(
                (
                    qi::eps >>
                    ((qi::lit('+') > qi::attr(true)) | qi::lit('-') > qi::attr(false))
                ) >
                u_entity_(sl::_r1)
            )
        ];
        debug_(bop_add_family_);

        bop_pow_.name("bop-pow");
        bop_pow_ = qi::skip [
            u_entity_(sl::_r1) >> qi::lit('^') >>
            (
                qi::eps >
                u_entity_(sl::_r1) % qi::lit('^')
            )
        ];
        debug_(bop_pow_);

        bop_assign_.name("bop-assign");
        bop_assign_ = qi::skip(ns::blank) [
            ((uop_subscript_(sl::_r1) | var_(sl::_r1)) >> qi::lit('=')) >
            u_entity_(sl::_r1)
        ];
        debug_(bop_assign_);

        // -------------------------------------------------------
        // groups
        group_.name("group");
        group_ =
            qi::lit('.') > (
                group_id_ >> -additional_class_(sl::_r1)
            ) [sl::_val = (*sl::_r1)[phx::construct<ast::detail::LookupQuery<ast::Group>>(sl::_r2, sl::_1, sl::_2)]]
        ;
        debug_(group_);

        endpoint_.name("endpoint");
        endpoint_ =
            qi::lit('#') > (
                endpoint_id_ >> -additional_class_(sl::_r1)
            ) [sl::_val = (*sl::_r1)[phx::construct<ast::detail::LookupQuery<ast::Endpoint>>(sl::_1, sl::_2)]]
        ;
        debug_(endpoint_);

        // -------------------------------------------------------
        // placeholdable (meta)
        placeholdable_.name("placeholdable");
        placeholdable_ = kw_inherit_ | kw_eq_;

        // -------------------------------------------------------
        // keywords
        kw_inherit_.name("kw-inherit");
        kw_inherit_ =
            qi::lit('_') >>
            qi::attr(zed::lazy_nullptr<ast::kw::Inherit const*>())
        ;
        debug_(kw_inherit_);

        kw_eq_.name("kw-eq");
        kw_eq_ =
            qi::lit('=') >>
            qi::attr(zed::lazy_nullptr<ast::kw::Eq const*>())
        ;
        debug_(kw_eq_);

        // -------------------------------------------------------
        // geo, block
        geo_.name("geo");
        geo_ = (
            qi::lexeme [
                qi::lit('$') >
                (
                    (qi::lit("hor-rev") >> qi::attr(ast::Geo::Dir::HOR_REV)) |
                    (qi::lit("hor") >> qi::attr(ast::Geo::Dir::HOR)) |
                    (qi::lit("ver-rev") >> qi::attr(ast::Geo::Dir::VER_REV)) |
                    (qi::lit("ver") >> qi::attr(ast::Geo::Dir::VER)) |
                    (qi::attr(ast::Geo::Dir::HOR))
                )
            ] >
            qi::skip(ns::blank) [
                (
                    qi::lit('+') > (
                        (qi::lit("start") >> qi::attr(ast::Geo::Justify::START)) |
                        (qi::lit("end") >> qi::attr(ast::Geo::Justify::END)) |
                        (qi::lit("center") >> qi::attr(ast::Geo::Justify::CENTER)) |
                        (qi::lit("expand") >> qi::attr(ast::Geo::Justify::EXPAND))
                    )
                ) |
                qi::attr(phx::val(ast::Geo::Justify::EXPAND))
            ] >
            qi::skip(ns::blank) [
                -attribute_(sl::_r1)
            ] >
            -qi::skip(ns::blank) [
                block_(sl::_r1)
            ]
        ) [sl::_val = (*sl::_r1)[zed::make_unique<ast::Geo>(sl::_1, sl::_2, sl::_3, sl::_4)]];
        debug_(geo_);

        block_.name("block");
        block_ = (
            (
                (qi::lit('{') >>
                -block_param_(sl::_r1) >> qi::skip(ns::space) [qi::eps]) >
                ((stmt_(sl::_r1) - qi::lit('}')) % stmt_break_)
            ) >
            qi::skip(ns::space) [qi::lit('}')]
        ) [sl::_val = (*sl::_r1)[zed::make_unique<ast::Block>(sl::_1, sl::_2)]];
        debug_(block_);

        block_param_.name("block-param");
        block_param_ =
        (
            qi::lit('|') >>
            argument_(sl::_r1) % qi::lit(',')
        ) >
        qi::lit('|')
        ;
        debug_(block_param_);

        // -------------------------------------------------------
        // lits
        #include "saya/ika/vm/internal_def.hpp"

        #define SAYA_DEF_I(unused0, lit_name, unused1) \
        BOOST_PP_CAT(BOOST_PP_CAT(lit_, lit_name), _).name( \
            "lit-" BOOST_PP_STRINGIZE(lit_name) \
            );

        #define SAYA_DEF(r, data, elem) \
            SAYA_DEF_I elem

        BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, SAYA_IKA_VM_LIT_TYPEMAP)
        #undef SAYA_DEF_I
        #undef SAYA_DEF

        #include "saya/ika/vm/internal_undef.hpp"

        lit_Map_.name("lit-Map");
        lit_Map_ =
            qi::lit('{') > qi::skip(ns::space) [
                *(
                    (
                        qi::lexeme [lit_Symbol_(sl::_r1) > qi::lit(ast::lit::Symbol::MAGIC_TOKEN())] >
                        expr_(sl::_r1)
                    ) [
                        sl::_a[*sl::_1] = sl::_2
                    ]
                    % qi::lit(',')
                )
            ] >
            qi::lit('}') >
            qi::eps [sl::_val = (*sl::_r1)[zed::make_unique<ast::lit::Map>(sl::_a)]]
        ;
        debug_(lit_Map_);

        // ---
        lit_Symbol_ = qi::as_string [
            (ns::alpha - (qi::char_('-') | qi::char_(ast::lit::Symbol::MAGIC_TOKEN()))) >>
            *((ns::alnum | qi::char_('_')) - (qi::char_('-') | qi::char_(ast::lit::Symbol::MAGIC_TOKEN())))
        ] [sl::_val = (*sl::_r1)[zed::make_unique<ast::lit::Symbol>(sl::_1)]]
        ;

        lit_String_ =
            sq_string_ [sl::_val = (*sl::_r1)[zed::make_unique<ast::lit::String>(sl::_1)]]
        ;

        // store color code in 32bit unsigned 0xRRGGBBAA format.
        // %= is important here!!
        lit_Color_ %=
            qi::lit(ast::lit::Color::MAGIC_TOKEN()) > (
                // #RRGGBBAA
                // hex_of_rrggbbaa_ [sl::_val = sl::_1] |

                // #RRGGBB
                hex_of_rrggbb_ [sl::_1 = (sl::_1 << 8) | 0xFF] |

                // #RGB
                hex_of_rgb_ [
                    (sl::_1 =
                        ((sl::_1 & 0xF00) << 16) |
                        ((sl::_1 & 0x0F0) << 12) |
                        ((sl::_1 & 0x00F) << 8)
                    ),
                    (sl::_1 |=
                        (sl::_1 << 4) | 0xFF
                    )
                ] |

                (qi::lit("transparent") >> qi::attr(0x00000000))
            )
        ;

        lit_Px_ =
            zt::uint64_ >> qi::lit("px")
        ;

        lit_Pct_ =
            zt::pct_ >> qi::lit('%')
        ;

        lit_Int64_ =
            &qi::lit(ast::lit::Int64::MAGIC_TOKEN()) >> zt::int64_
        ;

        lit_UInt64_ =
            zt::uint64_
        ;

        lit_Real_ =
            zt::real_
        ;

        // ---

        #include "saya/ika/vm/internal_def.hpp"

        #define SAYA_DEF_I(unused0, lit_name, unused1) \
            debug_(BOOST_PP_CAT(BOOST_PP_CAT(lit_, lit_name), _));

        #define SAYA_DEF(r, data, elem) \
            SAYA_DEF_I elem

        BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, SAYA_IKA_VM_LIT_TYPEMAP)
        #undef SAYA_DEF_I
        #undef SAYA_DEF

        #include "saya/ika/vm/internal_undef.hpp"

        // -------------------------------------------------------
        // basic
        namespace_.name("namespace");
        namespace_ =
            ((
                qi::lit("namespace") >
                ns_id_ [sl::_val = (*sl::_r1)[phx::construct<ast::detail::LookupQuery<ast::Namespace>>(sl::_1)]] >
                qi::lit('{')
            ) >> qi::skip(ns::space) [qi::eps] >>
                ((stmt_(sl::_r1) - qi::lit('}')) [phx::push_back(phx::at_c<1>(*sl::_val), sl::_1)] % stmt_break_)
            ) >
            qi::skip(ns::space) [qi::lit('}')] >
            qi::eps [(*sl::_r1)[ast::Root::Context::NAMESPACE]]
        ;
        debug_(namespace_);

        var_.name("var");
        var_ =
            qi::lit('%') >
            var_id_ [sl::_val = (*sl::_r1)[phx::construct<ast::detail::LookupQuery<ast::Var>>(sl::_1)]]
        ;
        debug_(var_);

        func_.name("func");
        func_ =
            qi::lit('@') >
            func_id_ [sl::_val = (*sl::_r1)[phx::construct<ast::detail::LookupQuery<ast::Func>>(sl::_1)]]
        ;
        debug_(func_);

        macro_.name("macro");
        macro_ =
            qi::lit('&') >
            macro_id_ [sl::_val = (*sl::_r1)[phx::construct<ast::detail::LookupQuery<ast::Macro>>(sl::_1)]]
        ;
        debug_(macro_);

        call_param_.name("call-param");
        call_param_ =
            (qi::lit('(') >>
            qi::skip(ns::space) [-(expr_(sl::_r1) % qi::lit(','))]) >
            qi::skip(ns::space) [qi::lit(')')]
        ;
        debug_(call_param_);

        // -------------------------------------------------------
        // special syntax
        default_specifier_.name("default-specifier");
        default_specifier_ =
            qi::lit("default") >
            geo_(sl::_r1)
        ;
        debug_(default_specifier_);

        argument_.name("argument");
        argument_ =
            var_(sl::_r1) [phx::at_c<0>(sl::_val) = sl::_1] >>
            -(qi::lit('=') > qi::skip(ns::blank) [expr_(sl::_r1) [phx::at_c<1>(sl::_val) = sl::_1]])
        ;
        debug_(argument_);

        // -------------------------------------------------------
        // meta

        // identifiers starting with '_' - reserved
        raw_id_.name("raw-id");
        raw_id_ =
            ns::alpha >> *(ns::alnum | qi::char_('_'))
        ;
        // debug_(raw_id_);

        raw_group_id_.name("raw-group-id");
        raw_group_id_ =
            ns::alpha >> *(ns::alnum | qi::char_('-'))
        ;

        ns_id_.name("ns-id"); // just a single namespace
        ns_id_ = qi::as_string [raw_id_];
        debug_(ns_id_);

        group_id_.name("group-id");
        group_id_ = qi::as_string [raw_group_id_];
        debug_(group_id_);

        endpoint_id_.name("endpoint-id");
        endpoint_id_ = qi::as_string [raw_group_id_];
        debug_(endpoint_id_);

        var_id_.name("var-id");
        var_id_ =
            qi::as_string [raw_id_]
        ;
        debug_(var_id_);

        func_id_.name("func-id");
        func_id_ =
            qi::as_string [raw_id_]
        ;
        debug_(func_id_);

        macro_id_.name("macro-id");
        macro_id_ =
            qi::as_string [raw_id_]
        ;
        debug_(macro_id_);

        // -------------------------------------------------------
        // utils
        sq_string_.name("sq-string");
        sq_string_ = qi::as_string[
            (qi::omit    [qi::char_('\'')[sl::_a = sl::_1]] >>
            qi::no_skip [*((qi::char_('\\') >> &qi::char_('\'')) | (qi::char_ - qi::char_(sl::_a)))]) >>
            qi::lit(sl::_a)
        ];
        debug_(sq_string_);

        // -------------------------------------------------------
        qi::on_error<qi::fail>(
            root_,
            zed::lazy_error_handler(sl::_1, sl::_2, sl::_3, sl::_4)
        );
    }

private:
    template<class Rule>
    static inline void debug_(Rule& r)
    {
        zed::debug<ast::io::variant_printer>(std::cerr, r);
    }

    template<class... Args>
    using rule = boost::spirit::qi::rule<Args...>;

    template<class... Args>
    using locals = boost::spirit::qi::locals<Args...>;

    using unused_type = boost::spirit::qi::unused_type;

    // ----------------------------------------------------
    rule<iterator_type, ast::Stmt(ast::Root*), blank_type>
    stmt_;

    rule<iterator_type, blank_type>
    empty_stmt_;

    rule<iterator_type, blank_type>
    stmt_break_;

    rule<iterator_type, ast::PrimaryExpr(ast::Root*)>
    primary_expr_;

    rule<iterator_type, ast::SideEffectExpr(ast::Root*)>
    se_expr_;

    rule<iterator_type, ast::Expr(ast::Root*)>
    expr_;

    rule<iterator_type, ast::Declaration(ast::Root*), blank_type>
    declaration_;

    rule<iterator_type, ast::FuncDefinition(ast::Root*), blank_type>
    func_definition_;

    rule<iterator_type, ast::MacroDefinition(ast::Root*), blank_type>
    macro_definition_;

    rule<iterator_type, ast::GroupChildSpecifier(ast::Root*)>
    group_child_specifier_;

    rule<iterator_type, ast::GroupDefinition(ast::Root*), blank_type>
    group_definition_;

    rule<iterator_type, ast::AdditionalClass*(ast::Root*)>
    additional_class_;

    rule<iterator_type, ast::Attribute*(ast::Root*), blank_type, locals<std::vector<ast::Attribute::attr_type>>>
    attribute_;

    // -------------------------------------------------------
    // geo, block
    rule<iterator_type, ast::Geo*(ast::Root*), blank_type>
    geo_;

    rule<iterator_type, ast::Block*(ast::Root*), blank_type>
    block_;

    rule<iterator_type, ast::BlockParam(ast::Root*), blank_type>
    block_param_;

    // -------------------------------------------------------
    // special syntax
    rule<iterator_type, ast::DefaultSpecifier(ast::Root*), blank_type>
    default_specifier_;

    rule<iterator_type, ast::Argument(ast::Root*), blank_type>
    argument_;

    // -------------------------------------------------------
    // op meta
    rule<iterator_type, ast::UEntity(ast::Root*)>
    u_entity_;

    // -------------------------------------------------------
    // uop
    rule<iterator_type, ast::UOp<ast::ops::FuncCall>(ast::Root*), blank_type>
    uop_func_call_;

    rule<iterator_type, ast::UOp<ast::ops::MacroCall>(ast::Root*), blank_type>
    uop_macro_call_;

    rule<iterator_type, ast::UOp<ast::ops::AddFamily>(ast::Root*)>
    uop_add_family_;

    rule<iterator_type, ast::UOp<ast::ops::Not>(ast::Root*)>
    uop_not_;

    rule<iterator_type, ast::UOp<ast::ops::Subscript>(ast::Root*), blank_type>
    uop_subscript_;

    // -------------------------------------------------------
    // bop
    rule<iterator_type, ast::BOp<ast::ops::Pow>(ast::Root*)>
    bop_pow_;

    rule<iterator_type, ast::BOp<ast::ops::MulFamily>(ast::Root*)>
    bop_mul_family_;

    rule<iterator_type, ast::BOp<ast::ops::AddFamily>(ast::Root*)>
    bop_add_family_;

    rule<iterator_type, ast::BOp<ast::ops::Assign>(ast::Root*)>
    bop_assign_;

    // -------------------------------------------------------
    // groups
    rule<iterator_type, ast::Group*(ast::Root*, bool)>
    group_;

    rule<iterator_type, ast::Endpoint*(ast::Root*)>
    endpoint_;

    // -------------------------------------------------------
    // placeholdable (meta)
    rule<iterator_type, ast::Placeholdable(ast::Root*), blank_type>
    placeholdable_;

    // -------------------------------------------------------
    // keywords
    rule<iterator_type, ast::kw::Inherit const*(), blank_type>
    kw_inherit_;

    rule<iterator_type, ast::kw::Eq const*(), blank_type>
    kw_eq_;

    // -------------------------------------------------------
    // lits
    rule<iterator_type, ast::lit::Map*(ast::Root*), blank_type, locals<ast::lit::Map::internal_type>>
    lit_Map_;

    rule<iterator_type, ast::lit::String*(ast::Root*), blank_type> // pass-by-pointer
    lit_String_;

    rule<iterator_type, ast::lit::Symbol*(ast::Root*)>
    lit_Symbol_;

    rule<iterator_type, ast::lit::Int64(ast::Root*), blank_type>
    lit_Int64_;

    rule<iterator_type, ast::lit::UInt64(ast::Root*), blank_type>
    lit_UInt64_;

    rule<iterator_type, ast::lit::Real(ast::Root*), blank_type>
    lit_Real_;

    rule<iterator_type, ast::lit::Color(ast::Root*), blank_type>
    lit_Color_;

    rule<iterator_type, ast::lit::Px(ast::Root*), blank_type>
    lit_Px_;

    rule<iterator_type, ast::lit::Pct(ast::Root*), blank_type>
    lit_Pct_;

    // -------------------------------------------------------
    // basic
    rule<iterator_type, ast::Namespace*(ast::Root*), blank_type>
    namespace_;

    rule<iterator_type, ast::Var*(ast::Root*), blank_type>
    var_;

    rule<iterator_type, ast::Func*(ast::Root*)>
    func_;

    rule<iterator_type, ast::Macro*(ast::Root*)>
    macro_;

    rule<iterator_type, ast::CallParam(ast::Root*), blank_type>
    call_param_;

    // -------------------------------------------------------
    // meta
    rule<iterator_type, std::string()>
    raw_id_, raw_group_id_;

    rule<iterator_type, ast::NSID()>
    ns_id_;

    rule<iterator_type, ast::GroupID()>
    group_id_;

    rule<iterator_type, ast::EndpointID()>
    endpoint_id_;

    rule<iterator_type, ast::VarID()>
    var_id_;

    rule<iterator_type, ast::FuncID()>
    func_id_;

    rule<iterator_type, ast::MacroID()>
    macro_id_;

    // -------------------------------------------------------
    // utils & trivial stuff
    rule<iterator_type, std::string(), locals<char>>
    sq_string_;
};

to_ast::to_ast()
    : to_ast::base_type(root_, "Ika")
    , impl_(std::make_unique<to_ast::Impl>(root_))
{}

// pimpl
to_ast::~to_ast() = default;

}}} // saya


#if 0
#undef SAYA_IKA_DESTRUCTOR_REGISTER
#undef SAYA_IKA_DETAIL_DESTRUCTOR_CALL
#undef SAYA_IKA_DESTRUCTOR_SCOPE
#endif
