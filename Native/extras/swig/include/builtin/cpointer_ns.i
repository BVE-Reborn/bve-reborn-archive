/* -----------------------------------------------------------------------------
 * cpointer.i
 *
 * SWIG library file containing macros that can be used to manipulate simple
 * pointer objects.
 * ----------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 * %pointer_class(type,name)
 *
 * Places a simple proxy around a simple type like 'int', 'float', or whatever.
 * The proxy provides this interface:
 *
 *       class type {
 *       public:
 *           type();
 *          ~type();
 *           type value();
 *           void assign(type value);
 *       };
 *         
 * Example:
 *
 *    %pointer_class(int, intp);
 *
 *    int add(int *x, int *y) { return *x + *y; }
 *
 * In python (with proxies)
 *
 *    >>> a = intp()
 *    >>> a.assign(10)
 *    >>> a.value()
 *    10
 *    >>> b = intp()
 *    >>> b.assign(20)
 *    >>> print add(a,b)
 *    30
 *
 * As a general rule, this macro should not be used on class/structures that
 * are already defined in the interface.
 * ----------------------------------------------------------------------------- */


%define %pointer_class_namespace(NAMESPACE, TYPE, NAME)
%{
namespace NAMESPACE {
  using NAME = TYPE;
}
%}

namespace NAMESPACE {
  typedef struct {
  } NAME;
}

%extend NAMESPACE::NAME {
#ifdef __cplusplus
NAME() {
  return new TYPE();
}
~NAME() {
  if ($self) delete $self;
}
#else
NAME() {
  return (TYPE *) calloc(1,sizeof(NAMESPACE::TYPE));
}
~NAME() {
  if ($self) free($self);
}
#endif
}

%extend NAMESPACE::NAME {

void assign(TYPE value) {
  *$self = value;
}
TYPE value() {
  return *$self;
}
TYPE * cast() {
  return $self;
}
static NAMESPACE::NAME * frompointer(TYPE *t) {
  return (NAMESPACE::NAME *) t;
}

}

%types(NAME = TYPE);

%enddef

/* -----------------------------------------------------------------------------
 * %pointer_cast(type1,type2,name)
 *
 * Generates a pointer casting function.
 * ----------------------------------------------------------------------------- */

%define %pointer_cast_namespace(NAMESPACE1, TYPE1, NAMESPACE2, TYPE2, NAME)
%inline %{
NAMESPACE2::TYPE2 NAME(NAMESPACE1::TYPE1 x) {
   return (NAMESPACE2::TYPE2) x;
}
%}
%enddef








