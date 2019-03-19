// Automatically generated

// hdr/includes
#ifndef SymEngineParserBase_h_included
#define SymEngineParserBase_h_included

#include <exception>
#include <vector>
#include <iostream>
// $insert polyincludes
#include <memory>
// $insert preincludes
#include "symengine/visitor.h"

// hdr/baseclass

namespace // anonymous
{
struct PI__;
}

// $insert namespace-open
namespace SymEngine
{

// $insert polymorphic
enum class Tag__ {
    basic,
    string,
    basic_vec,
};

namespace Meta__
{

extern size_t const *t_nErrors;

extern size_t const *s_nErrors__;

template <Tag__ tag>
struct TypeOf;

template <typename Tp_>
struct TagOf;

// $insert polymorphicSpecializations
enum { sizeofTag__ = 3 };

extern char const *idOfTag__[];
template <>
struct TagOf<RCP<const Basic>> {
    static Tag__ const tag = Tag__::basic;
};

template <>
struct TagOf<std::string> {
    static Tag__ const tag = Tag__::string;
};

template <>
struct TagOf<vec_basic> {
    static Tag__ const tag = Tag__::basic_vec;
};

template <>
struct TypeOf<Tag__::basic> {
    typedef RCP<const Basic> type;
};

template <>
struct TypeOf<Tag__::string> {
    typedef std::string type;
};

template <>
struct TypeOf<Tag__::basic_vec> {
    typedef vec_basic type;
};

// Individual semantic value classes are derived from Base, offering a
// member returning the value's Tag__, a member cloning the object of its
// derived Semantic<Tag__> and a member returning a pointerr to its
// derived Semantic<Tag__> data. See also Bisonc++'s distribution file
// README.polymorphic-techical
class Base
{
protected:
    Tag__ d_baseTag; // d_baseTag is assigned by Semantic.

public:
    Base() = default;
    Base(Base const &other) = delete;

    virtual ~Base();

    Tag__ tag() const;
    Base *clone() const;
    void *data() const;

private:
    virtual Base *vClone() const = 0;
    virtual void *vData() const = 0;
};

inline Base *Base::clone() const
{
    return vClone();
}

inline void *Base::data() const
{
    return vData();
}

inline Tag__ Base::tag() const
{
    return d_baseTag;
}

// The class Semantic stores a semantic value of the type matching tg_
template <Tag__ tg_>
class Semantic : public Base
{
    typename TypeOf<tg_>::type d_data;

public:
    Semantic();
    Semantic(Semantic<tg_> const &other); // req'd for cloning

    // This constructor member template forwards its arguments to
    // d_data, allowing it to be initialized using whatever
    // constructor is available for DataType
    template <typename... Params>
    Semantic(Params &&... params);

private:
    Base *vClone() const override;
    void *vData() const override;
};

template <Tag__ tg_>
Semantic<tg_>::Semantic()
{
    d_baseTag = tg_; // Base's data member:
}

template <Tag__ tg_>
Semantic<tg_>::Semantic(Semantic<tg_> const &other) : d_data(other.d_data)
{
    d_baseTag = other.d_baseTag;
}

template <Tag__ tg_>
template <typename... Params>
Semantic<tg_>::Semantic(Params &&... params)
    : d_data(std::forward<Params>(params)...)
{
    d_baseTag = tg_;
}

template <Tag__ tg_>
Base *Semantic<tg_>::vClone() const
{
    return new Semantic<tg_>{*this};
}

template <Tag__ tg_>
void *Semantic<tg_>::vData() const
{
    return const_cast<typename TypeOf<tg_>::type *>(&d_data);
}

// The class SType wraps a pointer to Base.  It becomes the polymorphic
// STYPE__ type. It also defines get members, allowing constructions like
// $$.get<INT> to be used.
class SType : private std::unique_ptr<Base>
{
    typedef std::unique_ptr<Base> BasePtr;

public:
    SType() = default;
    SType(SType const &other);
    SType(SType &&tmp);

    ~SType() = default;

    // Specific overloads are needed for SType = SType assignments
    SType &operator=(SType const &rhs);
    SType &operator=(SType &rhs); // required so it is used
                                  // instead of the template op=
    SType &operator=(SType &&tmp);

    // A template member operator= can be used when the compiler is
    // able to deduce the appropriate typename. Otherwise use assign.
    template <typename Type>
    SType &operator=(Type const &value);

    template <typename Type> // same, now moving
    SType &operator=(Type &&tmp);

    template <Tag__ tag, typename... Args>
    void assign(Args &&... args);

    // By default the get()-members check whether the specified <tag>
    // matches the tag returned by SType::tag (d_data's tag). If they
    // don't match a run-time fatal error results.
    template <Tag__ tag>
    typename TypeOf<tag>::type &get();

    template <Tag__ tag>
    typename TypeOf<tag>::type const &get() const;

    Tag__ tag() const;
    bool valid() const;
};

inline SType::SType(SType const &other) : BasePtr{other ? other->clone() : 0}
{
}

inline SType::SType(SType &&tmp) : BasePtr{std::move(tmp)}
{
}

inline SType &SType::operator=(SType const &rhs)
{
    reset(rhs->clone());
    return *this;
}

inline SType &SType::operator=(SType &rhs)
{
    reset(rhs->clone());
    return *this;
}

inline SType &SType::operator=(SType &&tmp)
{
    BasePtr::operator=(std::move(tmp));
    return *this;
}

// A template assignment function can be used when the compiler is
// able to deduce the appropriate typename
template <typename Type>
inline SType &SType::operator=(Type const &value)
{
    assign<TagOf<Type>::tag>(value);
    return *this;
}

template <typename Type>
inline SType &SType::operator=(Type &&tmp)
{
    assign<TagOf<typename std::remove_reference<Type>::type>::tag>(
        std::move(tmp));

    return *this;
}

template <Tag__ tag, typename... Args>
void SType::assign(Args &&... args)
{
    reset(new Semantic<tag>(std::forward<Args>(args)...));
}

template <Tag__ tg>
typename TypeOf<tg>::type &SType::get()
{
    // $insert warnTagMismatches

    if (tag() != tg) {
        if (*t_nErrors != 0)
            const_cast<SType *>(this)->assign<tg>();
        else {
            std::cerr
                << "[Fatal] calling `.get<Tag__::"
                << idOfTag__[static_cast<int>(tg)] << ">()', but Tag "
                << idOfTag__[static_cast<int>(tag())]
                << " is encountered. Try "
                   "option --debug and call setDebug(Parser::ACTIONCASES)\n";
            throw 1; // ABORTs
        }
    }

    return *static_cast<typename TypeOf<tg>::type *>((*this)->data());
}

template <Tag__ tg>
typename TypeOf<tg>::type const &SType::get() const
{
    // $insert warnTagMismatches

    if (tag() != tg) {
        if (*t_nErrors != 0)
            const_cast<SType *>(this)->assign<tg>();
        else {
            std::cerr
                << "[Fatal] calling `.get<Tag__::"
                << idOfTag__[static_cast<int>(tg)] << ">()', but Tag "
                << idOfTag__[static_cast<int>(tag())]
                << " is encountered. Try "
                   "option --debug and call setDebug(Parser::ACTIONCASES)\n";
            throw 1; // ABORTs
        }
    }

    return *static_cast<typename TypeOf<tg>::type *>((*this)->data());
}

inline Tag__ SType::tag() const
{
    return valid() ? (*this)->tag() : static_cast<Tag__>(sizeofTag__);
}

inline bool SType::valid() const
{
    return BasePtr::get() != 0;
}

} // namespace Meta__

class ParserBase
{
public:
    enum DebugMode__ { OFF = 0, ON = 1 << 0, ACTIONCASES = 1 << 1 };

    // $insert tokens

    // Symbolic tokens:
    enum Tokens__ {
        IDENTIFIER = 257,
        NUMERIC,
        IMPLICIT_MUL,
        EQ,
        LE,
        GE,
        UMINUS,
        POW,
        NOT,
    };

    // $insert STYPE
    typedef Meta__::SType STYPE__;

private:
    // state  semval
    typedef std::pair<size_t, STYPE__> StatePair;
    // token   semval
    typedef std::pair<int, STYPE__> TokenPair;

    int d_stackIdx = -1;
    std::vector<StatePair> d_stateStack;
    StatePair *d_vsp = 0; // points to the topmost value stack
    size_t d_state = 0;

    TokenPair d_next;
    int d_token;

    bool d_terminalToken = false;
    bool d_recovery = false;

protected:
    enum Return__ {
        PARSE_ACCEPT__ = 0, // values used as parse()'s return values
        PARSE_ABORT__ = 1
    };
    enum ErrorRecovery__ {
        UNEXPECTED_TOKEN__,
    };

    bool d_actionCases__ = false; // set by options/directives
    bool d_debug__ = true;
    size_t d_requiredTokens__;
    size_t d_nErrors__; // initialized by clearin()
    size_t d_acceptedTokens__;
    STYPE__ d_val__;

    ParserBase();

    void ABORT() const;
    void ACCEPT() const;
    void ERROR() const;

    STYPE__ &vs__(int idx); // value stack element idx
    int lookup__() const;
    int savedToken__() const;
    int token__() const;
    size_t stackSize__() const;
    size_t state__() const;
    size_t top__() const;
    void clearin__();
    void errorVerbose__();
    void lex__(int token);
    void popToken__();
    void pop__(size_t count = 1);
    void pushToken__(int token);
    void push__(size_t nextState);
    void redoToken__();
    bool recovery__() const;
    void reduce__(int rule);
    void shift__(int state);
    void startRecovery__();

public:
    void setDebug(bool mode);
    void setDebug(DebugMode__ mode);
};

// hdr/abort
inline void ParserBase::ABORT() const
{
    throw PARSE_ABORT__;
}

// hdr/accept
inline void ParserBase::ACCEPT() const
{
    throw PARSE_ACCEPT__;
}

// hdr/error
inline void ParserBase::ERROR() const
{
    throw UNEXPECTED_TOKEN__;
}

// hdr/savedtoken
inline int ParserBase::savedToken__() const
{
    return d_next.first;
}

// hdr/opbitand
inline ParserBase::DebugMode__ operator&(ParserBase::DebugMode__ lhs,
                                         ParserBase::DebugMode__ rhs)
{
    return static_cast<ParserBase::DebugMode__>(static_cast<int>(lhs) & rhs);
}

// hdr/opbitor
inline ParserBase::DebugMode__ operator|(ParserBase::DebugMode__ lhs,
                                         ParserBase::DebugMode__ rhs)
{
    return static_cast<ParserBase::DebugMode__>(static_cast<int>(lhs) | rhs);
};

// hdr/recovery
inline bool ParserBase::recovery__() const
{
    return d_recovery;
}

// hdr/stacksize
inline size_t ParserBase::stackSize__() const
{
    return d_stackIdx + 1;
}

// hdr/state
inline size_t ParserBase::state__() const
{
    return d_state;
}

// hdr/token
inline int ParserBase::token__() const
{
    return d_token;
}

// hdr/vs
inline ParserBase::STYPE__ &ParserBase::vs__(int idx)
{
    return (d_vsp + idx)->second;
}

// hdr/tail
// For convenience, when including ParserBase.h its symbols are available as
// symbols in the class Parser, too.
#define Parser ParserBase

// $insert namespace-close
}

#endif
