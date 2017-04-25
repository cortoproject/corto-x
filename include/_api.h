/* _api.h
 *
 * API convenience functions for C-language.
 * This file contains generated code. Do not modify!
 */

#ifndef CORTO_X__API_H
#define CORTO_X__API_H

#include <corto/corto.h>
#include <corto/x/_project.h>
#ifdef __cplusplus
extern "C" {
#endif
/* /corto/x/callback */
CORTO_X_EXPORT x_callback _x_callbackCreate(corto_type returnType, corto_bool returnsReference, corto_bool _virtual, void(*_impl)(void));
#define x_callbackCreate(returnType, returnsReference, _virtual, _impl) _x_callbackCreate(corto_type(returnType), returnsReference, _virtual, (void(*)(void))_impl)
#define x_callbackCreate_auto(_id, returnType, returnsReference, _virtual, _impl) x_callback _id = x_callbackCreate(returnType, returnsReference, _virtual, _impl); (void)_id
CORTO_X_EXPORT x_callback _x_callbackCreateChild(corto_object _parent, corto_string _id, corto_type returnType, corto_bool returnsReference, corto_bool _virtual, void(*_impl)(void));
#define x_callbackCreateChild(_parent, _id, returnType, returnsReference, _virtual, _impl) _x_callbackCreateChild(_parent, _id, corto_type(returnType), returnsReference, _virtual, (void(*)(void))_impl)
#define x_callbackCreateChild_auto(_parent, _id, returnType, returnsReference, _virtual, _impl) x_callback _id = x_callbackCreateChild(_parent, #_id, returnType, returnsReference, _virtual, _impl); (void)_id
CORTO_X_EXPORT corto_int16 _x_callbackUpdate(x_callback _this, corto_type returnType, corto_bool returnsReference, corto_bool _virtual, void(*_impl)(void));
#define x_callbackUpdate(_this, returnType, returnsReference, _virtual, _impl) _x_callbackUpdate(x_callback(_this), corto_type(returnType), returnsReference, _virtual, (void(*)(void))_impl)

CORTO_X_EXPORT x_callback _x_callbackDeclare(void);
#define x_callbackDeclare() _x_callbackDeclare()
#define x_callbackDeclare_auto(_id) x_callback _id = x_callbackDeclare(); (void)_id
CORTO_X_EXPORT x_callback _x_callbackDeclareChild(corto_object _parent, corto_string _id);
#define x_callbackDeclareChild(_parent, _id) _x_callbackDeclareChild(_parent, _id)
#define x_callbackDeclareChild_auto(_parent, _id) x_callback _id = x_callbackDeclareChild(_parent, #_id); (void)_id
CORTO_X_EXPORT corto_int16 _x_callbackDefine(x_callback _this, corto_type returnType, corto_bool returnsReference, corto_bool _virtual, void(*_impl)(void));
#define x_callbackDefine(_this, returnType, returnsReference, _virtual, _impl) _x_callbackDefine(x_callback(_this), corto_type(returnType), returnsReference, _virtual, (void(*)(void))_impl)
CORTO_X_EXPORT x_callback _x_callbackAssign(x_callback _this, corto_type returnType, corto_bool returnsReference, corto_bool _virtual, void(*_impl)(void));
#define x_callback__optional_NotSet NULL
#define x_callback__optional_Set(returnType, returnsReference, _virtual, _impl) x_callbackAssign((x_callback*)corto_calloc(sizeof(x_callback)), returnType, returnsReference, _virtual, _impl)
#define x_callback__optional_SetCond(cond, returnType, returnsReference, _virtual, _impl) cond ? x_callbackAssign((x_callback*)corto_calloc(sizeof(x_callback)), returnType, returnsReference, _virtual, _impl) : NULL
#define x_callbackUnset(_this) _this ? corto_deinitp(_this, x_callback_o) : 0; corto_dealloc(_this); _this = NULL;
#define x_callbackAssign(_this, returnType, returnsReference, _virtual, _impl) _x_callbackAssign(_this, corto_type(returnType), returnsReference, _virtual, (void(*)(void))_impl)
#define x_callbackSet(_this, returnType, returnsReference, _virtual, _impl) _this = _this ? _this : (x_callback*)corto_calloc(sizeof(x_callback)); _x_callbackAssign(_this, corto_type(returnType), returnsReference, _virtual, (void(*)(void))_impl)
CORTO_X_EXPORT corto_string _x_callbackStr(x_callback value);
#define x_callbackStr(value) _x_callbackStr(x_callback(value))
CORTO_X_EXPORT x_callback x_callbackFromStr(x_callback value, corto_string str);
CORTO_X_EXPORT corto_equalityKind _x_callbackCompare(x_callback dst, x_callback src);
#define x_callbackCompare(dst, src) _x_callbackCompare(x_callback(dst), x_callback(src))

/* /corto/x/parser */
CORTO_X_EXPORT x_parser _x_parserCreate(corto_interface base, corto_modifier baseAccess, corto_interfaceseq implements, corto_word ruleChain);
#define x_parserCreate(base, baseAccess, implements, ruleChain) _x_parserCreate(corto_interface(base), baseAccess, implements, ruleChain)
#define x_parserCreate_auto(_id, base, baseAccess, implements, ruleChain) x_parser _id = x_parserCreate(base, baseAccess, implements, ruleChain); (void)_id
CORTO_X_EXPORT x_parser _x_parserCreateChild(corto_object _parent, corto_string _id, corto_interface base, corto_modifier baseAccess, corto_interfaceseq implements, corto_word ruleChain);
#define x_parserCreateChild(_parent, _id, base, baseAccess, implements, ruleChain) _x_parserCreateChild(_parent, _id, corto_interface(base), baseAccess, implements, ruleChain)
#define x_parserCreateChild_auto(_parent, _id, base, baseAccess, implements, ruleChain) x_parser _id = x_parserCreateChild(_parent, #_id, base, baseAccess, implements, ruleChain); (void)_id
CORTO_X_EXPORT corto_int16 _x_parserUpdate(x_parser _this, corto_interface base, corto_modifier baseAccess, corto_interfaceseq implements, corto_word ruleChain);
#define x_parserUpdate(_this, base, baseAccess, implements, ruleChain) _x_parserUpdate(x_parser(_this), corto_interface(base), baseAccess, implements, ruleChain)

CORTO_X_EXPORT x_parser _x_parserDeclare(void);
#define x_parserDeclare() _x_parserDeclare()
#define x_parserDeclare_auto(_id) x_parser _id = x_parserDeclare(); (void)_id
CORTO_X_EXPORT x_parser _x_parserDeclareChild(corto_object _parent, corto_string _id);
#define x_parserDeclareChild(_parent, _id) _x_parserDeclareChild(_parent, _id)
#define x_parserDeclareChild_auto(_parent, _id) x_parser _id = x_parserDeclareChild(_parent, #_id); (void)_id
CORTO_X_EXPORT corto_int16 _x_parserDefine(x_parser _this, corto_interface base, corto_modifier baseAccess, corto_interfaceseq implements, corto_word ruleChain);
#define x_parserDefine(_this, base, baseAccess, implements, ruleChain) _x_parserDefine(x_parser(_this), corto_interface(base), baseAccess, implements, ruleChain)
CORTO_X_EXPORT x_parser _x_parserAssign(x_parser _this, corto_interface base, corto_modifier baseAccess, corto_interfaceseq implements, corto_word ruleChain);
#define x_parser__optional_NotSet NULL
#define x_parser__optional_Set(base, baseAccess, implements, ruleChain) x_parserAssign((x_parser*)corto_calloc(sizeof(x_parser)), base, baseAccess, implements, ruleChain)
#define x_parser__optional_SetCond(cond, base, baseAccess, implements, ruleChain) cond ? x_parserAssign((x_parser*)corto_calloc(sizeof(x_parser)), base, baseAccess, implements, ruleChain) : NULL
#define x_parserUnset(_this) _this ? corto_deinitp(_this, x_parser_o) : 0; corto_dealloc(_this); _this = NULL;
#define x_parserAssign(_this, base, baseAccess, implements, ruleChain) _x_parserAssign(_this, corto_interface(base), baseAccess, implements, ruleChain)
#define x_parserSet(_this, base, baseAccess, implements, ruleChain) _this = _this ? _this : (x_parser*)corto_calloc(sizeof(x_parser)); _x_parserAssign(_this, corto_interface(base), baseAccess, implements, ruleChain)
CORTO_X_EXPORT corto_string _x_parserStr(x_parser value);
#define x_parserStr(value) _x_parserStr(x_parser(value))
CORTO_X_EXPORT x_parser x_parserFromStr(x_parser value, corto_string str);
CORTO_X_EXPORT corto_equalityKind _x_parserCompare(x_parser dst, x_parser src);
#define x_parserCompare(dst, src) _x_parserCompare(x_parser(dst), x_parser(src))

/* /corto/x/pattern */
CORTO_X_EXPORT x_pattern _x_patternCreate(corto_string expr, corto_type type);
#define x_patternCreate(expr, type) _x_patternCreate(expr, corto_type(type))
#define x_patternCreate_auto(_id, expr, type) x_pattern _id = x_patternCreate(expr, type); (void)_id
CORTO_X_EXPORT x_pattern _x_patternCreateChild(corto_object _parent, corto_string _id, corto_string expr, corto_type type);
#define x_patternCreateChild(_parent, _id, expr, type) _x_patternCreateChild(_parent, _id, expr, corto_type(type))
#define x_patternCreateChild_auto(_parent, _id, expr, type) x_pattern _id = x_patternCreateChild(_parent, #_id, expr, type); (void)_id
CORTO_X_EXPORT corto_int16 _x_patternUpdate(x_pattern _this, corto_string expr, corto_type type);
#define x_patternUpdate(_this, expr, type) _x_patternUpdate(x_pattern(_this), expr, corto_type(type))

CORTO_X_EXPORT x_pattern _x_patternDeclare(void);
#define x_patternDeclare() _x_patternDeclare()
#define x_patternDeclare_auto(_id) x_pattern _id = x_patternDeclare(); (void)_id
CORTO_X_EXPORT x_pattern _x_patternDeclareChild(corto_object _parent, corto_string _id);
#define x_patternDeclareChild(_parent, _id) _x_patternDeclareChild(_parent, _id)
#define x_patternDeclareChild_auto(_parent, _id) x_pattern _id = x_patternDeclareChild(_parent, #_id); (void)_id
CORTO_X_EXPORT corto_int16 _x_patternDefine(x_pattern _this, corto_string expr, corto_type type);
#define x_patternDefine(_this, expr, type) _x_patternDefine(x_pattern(_this), expr, corto_type(type))
CORTO_X_EXPORT x_pattern _x_patternAssign(x_pattern _this, corto_string expr, corto_type type);
#define x_pattern__optional_NotSet NULL
#define x_pattern__optional_Set(expr, type) x_patternAssign((x_pattern*)corto_calloc(sizeof(x_pattern)), expr, type)
#define x_pattern__optional_SetCond(cond, expr, type) cond ? x_patternAssign((x_pattern*)corto_calloc(sizeof(x_pattern)), expr, type) : NULL
#define x_patternUnset(_this) _this ? corto_deinitp(_this, x_pattern_o) : 0; corto_dealloc(_this); _this = NULL;
#define x_patternAssign(_this, expr, type) _x_patternAssign(_this, expr, corto_type(type))
#define x_patternSet(_this, expr, type) _this = _this ? _this : (x_pattern*)corto_calloc(sizeof(x_pattern)); _x_patternAssign(_this, expr, corto_type(type))
CORTO_X_EXPORT corto_string _x_patternStr(x_pattern value);
#define x_patternStr(value) _x_patternStr(x_pattern(value))
CORTO_X_EXPORT x_pattern x_patternFromStr(x_pattern value, corto_string str);
CORTO_X_EXPORT corto_equalityKind _x_patternCompare(x_pattern dst, x_pattern src);
#define x_patternCompare(dst, src) _x_patternCompare(x_pattern(dst), x_pattern(src))

/* /corto/x/pattern/parameter */
CORTO_X_EXPORT x_pattern_parameter* _x_pattern_parameterCreate(corto_string name, corto_type type);
#define x_pattern_parameterCreate(name, type) _x_pattern_parameterCreate(name, corto_type(type))
#define x_pattern_parameterCreate_auto(_id, name, type) x_pattern_parameter* _id = x_pattern_parameterCreate(name, type); (void)_id
CORTO_X_EXPORT x_pattern_parameter* _x_pattern_parameterCreateChild(corto_object _parent, corto_string _id, corto_string name, corto_type type);
#define x_pattern_parameterCreateChild(_parent, _id, name, type) _x_pattern_parameterCreateChild(_parent, _id, name, corto_type(type))
#define x_pattern_parameterCreateChild_auto(_parent, _id, name, type) x_pattern_parameter* _id = x_pattern_parameterCreateChild(_parent, #_id, name, type); (void)_id
CORTO_X_EXPORT corto_int16 _x_pattern_parameterUpdate(x_pattern_parameter* _this, corto_string name, corto_type type);
#define x_pattern_parameterUpdate(_this, name, type) _x_pattern_parameterUpdate(x_pattern_parameter(_this), name, corto_type(type))

CORTO_X_EXPORT x_pattern_parameter* _x_pattern_parameterDeclare(void);
#define x_pattern_parameterDeclare() _x_pattern_parameterDeclare()
#define x_pattern_parameterDeclare_auto(_id) x_pattern_parameter* _id = x_pattern_parameterDeclare(); (void)_id
CORTO_X_EXPORT x_pattern_parameter* _x_pattern_parameterDeclareChild(corto_object _parent, corto_string _id);
#define x_pattern_parameterDeclareChild(_parent, _id) _x_pattern_parameterDeclareChild(_parent, _id)
#define x_pattern_parameterDeclareChild_auto(_parent, _id) x_pattern_parameter* _id = x_pattern_parameterDeclareChild(_parent, #_id); (void)_id
CORTO_X_EXPORT corto_int16 _x_pattern_parameterDefine(x_pattern_parameter* _this, corto_string name, corto_type type);
#define x_pattern_parameterDefine(_this, name, type) _x_pattern_parameterDefine(x_pattern_parameter(_this), name, corto_type(type))
CORTO_X_EXPORT x_pattern_parameter* _x_pattern_parameterAssign(x_pattern_parameter* _this, corto_string name, corto_type type);
#define x_pattern_parameter__optional_NotSet NULL
#define x_pattern_parameter__optional_Set(name, type) x_pattern_parameterAssign((x_pattern_parameter*)corto_calloc(sizeof(x_pattern_parameter)), name, type)
#define x_pattern_parameter__optional_SetCond(cond, name, type) cond ? x_pattern_parameterAssign((x_pattern_parameter*)corto_calloc(sizeof(x_pattern_parameter)), name, type) : NULL
#define x_pattern_parameterUnset(_this) _this ? corto_deinitp(_this, x_pattern_parameter_o) : 0; corto_dealloc(_this); _this = NULL;
#define x_pattern_parameterAssign(_this, name, type) _x_pattern_parameterAssign(_this, name, corto_type(type))
#define x_pattern_parameterSet(_this, name, type) _this = _this ? _this : (x_pattern_parameter*)corto_calloc(sizeof(x_pattern_parameter)); _x_pattern_parameterAssign(_this, name, corto_type(type))
CORTO_X_EXPORT corto_string _x_pattern_parameterStr(x_pattern_parameter* value);
#define x_pattern_parameterStr(value) _x_pattern_parameterStr(value)
CORTO_X_EXPORT x_pattern_parameter* x_pattern_parameterFromStr(x_pattern_parameter* value, corto_string str);
CORTO_X_EXPORT corto_equalityKind x_pattern_parameterCompare(x_pattern_parameter* dst, x_pattern_parameter* src);

CORTO_X_EXPORT corto_int16 _x_pattern_parameterInit(x_pattern_parameter* value);
#define x_pattern_parameterInit(value) _x_pattern_parameterInit(value)
CORTO_X_EXPORT corto_int16 _x_pattern_parameterDeinit(x_pattern_parameter* value);
#define x_pattern_parameterDeinit(value) _x_pattern_parameterDeinit(value)

/* /corto/x/rule */
CORTO_X_EXPORT x_rule _x_ruleCreate(corto_string pattern, corto_string regex, corto_word compiledRegex, void(*_impl)(void));
#define x_ruleCreate(pattern, regex, compiledRegex, _impl) _x_ruleCreate(pattern, regex, compiledRegex, (void(*)(void))_impl)
#define x_ruleCreate_auto(_id, pattern, regex, compiledRegex, _impl) x_rule _id = x_ruleCreate(pattern, regex, compiledRegex, _impl); (void)_id
CORTO_X_EXPORT x_rule _x_ruleCreateChild(corto_object _parent, corto_string _id, corto_string pattern, corto_string regex, corto_word compiledRegex, void(*_impl)(void));
#define x_ruleCreateChild(_parent, _id, pattern, regex, compiledRegex, _impl) _x_ruleCreateChild(_parent, _id, pattern, regex, compiledRegex, (void(*)(void))_impl)
#define x_ruleCreateChild_auto(_parent, _id, pattern, regex, compiledRegex, _impl) x_rule _id = x_ruleCreateChild(_parent, #_id, pattern, regex, compiledRegex, _impl); (void)_id
CORTO_X_EXPORT corto_int16 _x_ruleUpdate(x_rule _this, corto_string pattern, corto_string regex, corto_word compiledRegex, void(*_impl)(void));
#define x_ruleUpdate(_this, pattern, regex, compiledRegex, _impl) _x_ruleUpdate(x_rule(_this), pattern, regex, compiledRegex, (void(*)(void))_impl)

CORTO_X_EXPORT x_rule _x_ruleDeclare(void);
#define x_ruleDeclare() _x_ruleDeclare()
#define x_ruleDeclare_auto(_id) x_rule _id = x_ruleDeclare(); (void)_id
CORTO_X_EXPORT x_rule _x_ruleDeclareChild(corto_object _parent, corto_string _id);
#define x_ruleDeclareChild(_parent, _id) _x_ruleDeclareChild(_parent, _id)
#define x_ruleDeclareChild_auto(_parent, _id) x_rule _id = x_ruleDeclareChild(_parent, #_id); (void)_id
CORTO_X_EXPORT corto_int16 _x_ruleDefine(x_rule _this, corto_string pattern, corto_string regex, corto_word compiledRegex, void(*_impl)(void));
#define x_ruleDefine(_this, pattern, regex, compiledRegex, _impl) _x_ruleDefine(x_rule(_this), pattern, regex, compiledRegex, (void(*)(void))_impl)
CORTO_X_EXPORT x_rule _x_ruleAssign(x_rule _this, corto_string pattern, corto_string regex, corto_word compiledRegex, void(*_impl)(void));
#define x_rule__optional_NotSet NULL
#define x_rule__optional_Set(pattern, regex, compiledRegex, _impl) x_ruleAssign((x_rule*)corto_calloc(sizeof(x_rule)), pattern, regex, compiledRegex, _impl)
#define x_rule__optional_SetCond(cond, pattern, regex, compiledRegex, _impl) cond ? x_ruleAssign((x_rule*)corto_calloc(sizeof(x_rule)), pattern, regex, compiledRegex, _impl) : NULL
#define x_ruleUnset(_this) _this ? corto_deinitp(_this, x_rule_o) : 0; corto_dealloc(_this); _this = NULL;
#define x_ruleAssign(_this, pattern, regex, compiledRegex, _impl) _x_ruleAssign(_this, pattern, regex, compiledRegex, (void(*)(void))_impl)
#define x_ruleSet(_this, pattern, regex, compiledRegex, _impl) _this = _this ? _this : (x_rule*)corto_calloc(sizeof(x_rule)); _x_ruleAssign(_this, pattern, regex, compiledRegex, (void(*)(void))_impl)
CORTO_X_EXPORT corto_string _x_ruleStr(x_rule value);
#define x_ruleStr(value) _x_ruleStr(x_rule(value))
CORTO_X_EXPORT x_rule x_ruleFromStr(x_rule value, corto_string str);
CORTO_X_EXPORT corto_equalityKind _x_ruleCompare(x_rule dst, x_rule src);
#define x_ruleCompare(dst, src) _x_ruleCompare(x_rule(dst), x_rule(src))

/* /corto/x/token */
CORTO_X_EXPORT x_token _x_tokenCreate(corto_string regex, corto_type type);
#define x_tokenCreate(regex, type) _x_tokenCreate(regex, corto_type(type))
#define x_tokenCreate_auto(_id, regex, type) x_token _id = x_tokenCreate(regex, type); (void)_id
CORTO_X_EXPORT x_token _x_tokenCreateChild(corto_object _parent, corto_string _id, corto_string regex, corto_type type);
#define x_tokenCreateChild(_parent, _id, regex, type) _x_tokenCreateChild(_parent, _id, regex, corto_type(type))
#define x_tokenCreateChild_auto(_parent, _id, regex, type) x_token _id = x_tokenCreateChild(_parent, #_id, regex, type); (void)_id
CORTO_X_EXPORT corto_int16 _x_tokenUpdate(x_token _this, corto_string regex, corto_type type);
#define x_tokenUpdate(_this, regex, type) _x_tokenUpdate(x_token(_this), regex, corto_type(type))

CORTO_X_EXPORT x_token _x_tokenDeclare(void);
#define x_tokenDeclare() _x_tokenDeclare()
#define x_tokenDeclare_auto(_id) x_token _id = x_tokenDeclare(); (void)_id
CORTO_X_EXPORT x_token _x_tokenDeclareChild(corto_object _parent, corto_string _id);
#define x_tokenDeclareChild(_parent, _id) _x_tokenDeclareChild(_parent, _id)
#define x_tokenDeclareChild_auto(_parent, _id) x_token _id = x_tokenDeclareChild(_parent, #_id); (void)_id
CORTO_X_EXPORT corto_int16 _x_tokenDefine(x_token _this, corto_string regex, corto_type type);
#define x_tokenDefine(_this, regex, type) _x_tokenDefine(x_token(_this), regex, corto_type(type))
CORTO_X_EXPORT x_token _x_tokenAssign(x_token _this, corto_string regex, corto_type type);
#define x_token__optional_NotSet NULL
#define x_token__optional_Set(regex, type) x_tokenAssign((x_token*)corto_calloc(sizeof(x_token)), regex, type)
#define x_token__optional_SetCond(cond, regex, type) cond ? x_tokenAssign((x_token*)corto_calloc(sizeof(x_token)), regex, type) : NULL
#define x_tokenUnset(_this) _this ? corto_deinitp(_this, x_token_o) : 0; corto_dealloc(_this); _this = NULL;
#define x_tokenAssign(_this, regex, type) _x_tokenAssign(_this, regex, corto_type(type))
#define x_tokenSet(_this, regex, type) _this = _this ? _this : (x_token*)corto_calloc(sizeof(x_token)); _x_tokenAssign(_this, regex, corto_type(type))
CORTO_X_EXPORT corto_string _x_tokenStr(x_token value);
#define x_tokenStr(value) _x_tokenStr(x_token(value))
CORTO_X_EXPORT x_token x_tokenFromStr(x_token value, corto_string str);
CORTO_X_EXPORT corto_equalityKind _x_tokenCompare(x_token dst, x_token src);
#define x_tokenCompare(dst, src) _x_tokenCompare(x_token(dst), x_token(src))

/* /corto/x/visitor */
CORTO_X_EXPORT x_visitor _x_visitorCreate(x_parser parser);
#define x_visitorCreate(parser) _x_visitorCreate(x_parser(parser))
#define x_visitorCreate_auto(_id, parser) x_visitor _id = x_visitorCreate(parser); (void)_id
CORTO_X_EXPORT x_visitor _x_visitorCreateChild(corto_object _parent, corto_string _id, x_parser parser);
#define x_visitorCreateChild(_parent, _id, parser) _x_visitorCreateChild(_parent, _id, x_parser(parser))
#define x_visitorCreateChild_auto(_parent, _id, parser) x_visitor _id = x_visitorCreateChild(_parent, #_id, parser); (void)_id
CORTO_X_EXPORT corto_int16 _x_visitorUpdate(x_visitor _this, x_parser parser);
#define x_visitorUpdate(_this, parser) _x_visitorUpdate(x_visitor(_this), x_parser(parser))

CORTO_X_EXPORT x_visitor _x_visitorDeclare(void);
#define x_visitorDeclare() _x_visitorDeclare()
#define x_visitorDeclare_auto(_id) x_visitor _id = x_visitorDeclare(); (void)_id
CORTO_X_EXPORT x_visitor _x_visitorDeclareChild(corto_object _parent, corto_string _id);
#define x_visitorDeclareChild(_parent, _id) _x_visitorDeclareChild(_parent, _id)
#define x_visitorDeclareChild_auto(_parent, _id) x_visitor _id = x_visitorDeclareChild(_parent, #_id); (void)_id
CORTO_X_EXPORT corto_int16 _x_visitorDefine(x_visitor _this, x_parser parser);
#define x_visitorDefine(_this, parser) _x_visitorDefine(x_visitor(_this), x_parser(parser))
CORTO_X_EXPORT x_visitor _x_visitorAssign(x_visitor _this, x_parser parser);
#define x_visitor__optional_NotSet NULL
#define x_visitor__optional_Set(parser) x_visitorAssign((x_visitor*)corto_calloc(sizeof(x_visitor)), parser)
#define x_visitor__optional_SetCond(cond, parser) cond ? x_visitorAssign((x_visitor*)corto_calloc(sizeof(x_visitor)), parser) : NULL
#define x_visitorUnset(_this) _this ? corto_deinitp(_this, x_visitor_o) : 0; corto_dealloc(_this); _this = NULL;
#define x_visitorAssign(_this, parser) _x_visitorAssign(_this, x_parser(parser))
#define x_visitorSet(_this, parser) _this = _this ? _this : (x_visitor*)corto_calloc(sizeof(x_visitor)); _x_visitorAssign(_this, x_parser(parser))
CORTO_X_EXPORT corto_string _x_visitorStr(x_visitor value);
#define x_visitorStr(value) _x_visitorStr(x_visitor(value))
CORTO_X_EXPORT x_visitor x_visitorFromStr(x_visitor value, corto_string str);
CORTO_X_EXPORT corto_equalityKind _x_visitorCompare(x_visitor dst, x_visitor src);
#define x_visitorCompare(dst, src) _x_visitorCompare(x_visitor(dst), x_visitor(src))


/* list{object,0} */
#define corto_objectListForeach(list, elem) \
    corto_iter elem##_iter = corto_llIter(list);\
    corto_object elem;\
    while(corto_iterHasNext(&elem##_iter) ? elem = (corto_object)corto_iterNext(&elem##_iter), TRUE : FALSE)

CORTO_X_EXPORT void corto_objectListInsert(corto_objectList list, corto_object element);
CORTO_X_EXPORT void corto_objectListAppend(corto_objectList list, corto_object element);
CORTO_X_EXPORT void corto_objectListRemove(corto_objectList list, corto_object element);
CORTO_X_EXPORT corto_object corto_objectListTakeFirst(corto_objectList list);
CORTO_X_EXPORT corto_object corto_objectListLast(corto_objectList list);
CORTO_X_EXPORT corto_object corto_objectListGet(corto_objectList list, corto_uint32 index);
CORTO_X_EXPORT corto_uint32 corto_objectListSize(corto_objectList list);
CORTO_X_EXPORT void corto_objectListClear(corto_objectList list);

/* list{/corto/x/pattern/parameter,0} */
#define x_pattern_parameterListForeach(list, elem) \
    corto_iter elem##_iter = corto_llIter(list);\
    x_pattern_parameter elem;\
    while(corto_iterHasNext(&elem##_iter) ? elem = *(x_pattern_parameter*)(corto_word)corto_iterNext(&elem##_iter), TRUE : FALSE)

CORTO_X_EXPORT x_pattern_parameter* x_pattern_parameterListInsertAlloc(x_pattern_parameterList list);
CORTO_X_EXPORT x_pattern_parameter* x_pattern_parameterListInsert(x_pattern_parameterList list, x_pattern_parameter* element);
CORTO_X_EXPORT x_pattern_parameter* x_pattern_parameterListAppendAlloc(x_pattern_parameterList list);
CORTO_X_EXPORT x_pattern_parameter* x_pattern_parameterListAppend(x_pattern_parameterList list, x_pattern_parameter* element);
CORTO_X_EXPORT x_pattern_parameter* x_pattern_parameterListTakeFirst(x_pattern_parameterList list);
CORTO_X_EXPORT x_pattern_parameter* x_pattern_parameterListLast(x_pattern_parameterList list);
CORTO_X_EXPORT x_pattern_parameter* x_pattern_parameterListGet(x_pattern_parameterList list, corto_uint32 index);
CORTO_X_EXPORT corto_uint32 x_pattern_parameterListSize(x_pattern_parameterList list);
CORTO_X_EXPORT void x_pattern_parameterListClear(x_pattern_parameterList list);

#ifdef __cplusplus
}
#endif
#endif

