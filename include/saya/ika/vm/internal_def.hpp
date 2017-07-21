// no include guards.

#define SAYA_IKA_VM_INTERNAL_TYPEMAP \
    ((::saya::ika::vm::UndecidedType, Undecided)) \
    ((std::string, String)) \
    ((std::int32_t, I32)) \
    ((std::uint32_t, UI32)) \
    ((std::int64_t, I64)) \
    ((std::uint64_t, UI64)) \
    ((::boost::multiprecision::cpp_dec_float_50, Real)) \
    ((::saya::zed::upct, Pct))

#define SAYA_IKA_VM_INTERNAL_TYPE_FOR(kv) \
    BOOST_PP_TUPLE_ELEM(0, kv)

#define SAYA_IKA_VM_INTERNAL_NAME_FOR(kv) \
    BOOST_PP_TUPLE_ELEM(1, kv)

#define SAYA_IKA_VM_INTERNAL_TYPEMAP_TYPEONLY_DEF(r, _, kv) \
    (SAYA_IKA_VM_INTERNAL_TYPE_FOR(kv))

#define SAYA_IKA_VM_INTERNAL_TYPEMAP_TYPEONLY \
    BOOST_PP_SEQ_FOR_EACH(SAYA_IKA_VM_INTERNAL_TYPEMAP_TYPEONLY_DEF, _, SAYA_IKA_VM_INTERNAL_TYPEMAP)

#define SAYA_IKA_VM_INTERNAL_TYPEMAP_NAMEONLY_DEF(r, _, kv) \
    (SAYA_IKA_VM_INTERNAL_NAME_FOR(kv))

#define SAYA_IKA_VM_INTERNAL_TYPEMAP_NAMEONLY \
    BOOST_PP_SEQ_FOR_EACH(SAYA_IKA_VM_INTERNAL_TYPEMAP_NAMEONLY_DEF, _, SAYA_IKA_VM_INTERNAL_TYPEMAP)


#define SAYA_IKA_VM_LIT_TYPEMAP \
    ((1, String, true)) \
    ((4, Int64, false)) \
    ((5, UInt64, false)) \
    ((6, Real, false)) \
    ((3, Color, false)) \
    ((3, Px, false)) \
    ((7, Pct, false))

#define SAYA_IKA_VM_LIT_VTYPEMAP_FOR(vtmapid) \
    BOOST_PP_SEQ_ELEM(vtmapid, SAYA_IKA_VM_INTERNAL_TYPEMAP)

#define SAYA_IKA_VM_LIT_NEED_POINTER_true *
#define SAYA_IKA_VM_LIT_NEED_POINTER_false

#define SAYA_IKA_VM_LIT_NEED_POINTER_FOR(flag) \
    SAYA_IKA_VM_LIT_NEED_POINTER_ ## flag

#define SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY_DEF_I(unused0, lit_name, unused1) \
    (lit_name)

#define SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY_DEF(r, data, elem) \
    SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY_DEF_I elem

#define SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY \
    BOOST_PP_SEQ_FOR_EACH(SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY_DEF, _, SAYA_IKA_VM_LIT_TYPEMAP)
