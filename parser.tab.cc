// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.tab.hh"


// Unqualified %code blocks.
#line 16 "parser.yy"

  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;

  Node* root;
  extern int yylineno;

#line 54 "parser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 127 "parser.tab.cc"

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_program: // program
      case symbol_kind::S_pre_start: // pre_start
      case symbol_kind::S_class: // class
      case symbol_kind::S_classAttrbt: // classAttrbt
      case symbol_kind::S_entry: // entry
      case symbol_kind::S_method: // method
      case symbol_kind::S_arg_method: // arg_method
      case symbol_kind::S_paramList: // paramList
      case symbol_kind::S_type: // type
      case symbol_kind::S_var: // var
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_stmtBl: // stmtBl
      case symbol_kind::S_stmtList: // stmtList
      case symbol_kind::S_forArg1: // forArg1
      case symbol_kind::S_forArg3: // forArg3
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_arg: // arg
      case symbol_kind::S_values: // values
      case symbol_kind::S_postfix: // postfix
      case symbol_kind::S_primary: // primary
        value.YY_MOVE_OR_COPY< Node* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_LCB: // LCB
      case symbol_kind::S_RCB: // RCB
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_RELOP: // RELOP
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_PWR: // PWR
      case symbol_kind::S_LBR: // LBR
      case symbol_kind::S_RBR: // RBR
      case symbol_kind::S_DOR: // DOR
      case symbol_kind::S_COMA: // COMA
      case symbol_kind::S_QES: // QES
      case symbol_kind::S_EXM: // EXM
      case symbol_kind::S_ASS: // ASS
      case symbol_kind::S_INTTYPE: // INTTYPE
      case symbol_kind::S_FLOATTYPE: // FLOATTYPE
      case symbol_kind::S_BOOLTYPE: // BOOLTYPE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_VOLATILE: // VOLATILE
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_COL: // COL
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_NEWLINE: // NEWLINE
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_PRINT: // PRINT
      case symbol_kind::S_READ: // READ
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_FOR: // FOR
      case symbol_kind::S_BREAK: // BREAK
      case symbol_kind::S_CONTINUE: // CONTINUE
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_baseType: // baseType
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_program: // program
      case symbol_kind::S_pre_start: // pre_start
      case symbol_kind::S_class: // class
      case symbol_kind::S_classAttrbt: // classAttrbt
      case symbol_kind::S_entry: // entry
      case symbol_kind::S_method: // method
      case symbol_kind::S_arg_method: // arg_method
      case symbol_kind::S_paramList: // paramList
      case symbol_kind::S_type: // type
      case symbol_kind::S_var: // var
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_stmtBl: // stmtBl
      case symbol_kind::S_stmtList: // stmtList
      case symbol_kind::S_forArg1: // forArg1
      case symbol_kind::S_forArg3: // forArg3
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_arg: // arg
      case symbol_kind::S_values: // values
      case symbol_kind::S_postfix: // postfix
      case symbol_kind::S_primary: // primary
        value.move< Node* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_LCB: // LCB
      case symbol_kind::S_RCB: // RCB
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_RELOP: // RELOP
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_PWR: // PWR
      case symbol_kind::S_LBR: // LBR
      case symbol_kind::S_RBR: // RBR
      case symbol_kind::S_DOR: // DOR
      case symbol_kind::S_COMA: // COMA
      case symbol_kind::S_QES: // QES
      case symbol_kind::S_EXM: // EXM
      case symbol_kind::S_ASS: // ASS
      case symbol_kind::S_INTTYPE: // INTTYPE
      case symbol_kind::S_FLOATTYPE: // FLOATTYPE
      case symbol_kind::S_BOOLTYPE: // BOOLTYPE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_VOLATILE: // VOLATILE
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_COL: // COL
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_NEWLINE: // NEWLINE
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_PRINT: // PRINT
      case symbol_kind::S_READ: // READ
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_FOR: // FOR
      case symbol_kind::S_BREAK: // BREAK
      case symbol_kind::S_CONTINUE: // CONTINUE
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_baseType: // baseType
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_program: // program
      case symbol_kind::S_pre_start: // pre_start
      case symbol_kind::S_class: // class
      case symbol_kind::S_classAttrbt: // classAttrbt
      case symbol_kind::S_entry: // entry
      case symbol_kind::S_method: // method
      case symbol_kind::S_arg_method: // arg_method
      case symbol_kind::S_paramList: // paramList
      case symbol_kind::S_type: // type
      case symbol_kind::S_var: // var
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_stmtBl: // stmtBl
      case symbol_kind::S_stmtList: // stmtList
      case symbol_kind::S_forArg1: // forArg1
      case symbol_kind::S_forArg3: // forArg3
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_arg: // arg
      case symbol_kind::S_values: // values
      case symbol_kind::S_postfix: // postfix
      case symbol_kind::S_primary: // primary
        value.copy< Node* > (that.value);
        break;

      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_LCB: // LCB
      case symbol_kind::S_RCB: // RCB
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_RELOP: // RELOP
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_PWR: // PWR
      case symbol_kind::S_LBR: // LBR
      case symbol_kind::S_RBR: // RBR
      case symbol_kind::S_DOR: // DOR
      case symbol_kind::S_COMA: // COMA
      case symbol_kind::S_QES: // QES
      case symbol_kind::S_EXM: // EXM
      case symbol_kind::S_ASS: // ASS
      case symbol_kind::S_INTTYPE: // INTTYPE
      case symbol_kind::S_FLOATTYPE: // FLOATTYPE
      case symbol_kind::S_BOOLTYPE: // BOOLTYPE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_VOLATILE: // VOLATILE
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_COL: // COL
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_NEWLINE: // NEWLINE
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_PRINT: // PRINT
      case symbol_kind::S_READ: // READ
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_FOR: // FOR
      case symbol_kind::S_BREAK: // BREAK
      case symbol_kind::S_CONTINUE: // CONTINUE
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_baseType: // baseType
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_program: // program
      case symbol_kind::S_pre_start: // pre_start
      case symbol_kind::S_class: // class
      case symbol_kind::S_classAttrbt: // classAttrbt
      case symbol_kind::S_entry: // entry
      case symbol_kind::S_method: // method
      case symbol_kind::S_arg_method: // arg_method
      case symbol_kind::S_paramList: // paramList
      case symbol_kind::S_type: // type
      case symbol_kind::S_var: // var
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_stmtBl: // stmtBl
      case symbol_kind::S_stmtList: // stmtList
      case symbol_kind::S_forArg1: // forArg1
      case symbol_kind::S_forArg3: // forArg3
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_arg: // arg
      case symbol_kind::S_values: // values
      case symbol_kind::S_postfix: // postfix
      case symbol_kind::S_primary: // primary
        value.move< Node* > (that.value);
        break;

      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_LCB: // LCB
      case symbol_kind::S_RCB: // RCB
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_RELOP: // RELOP
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_PWR: // PWR
      case symbol_kind::S_LBR: // LBR
      case symbol_kind::S_RBR: // RBR
      case symbol_kind::S_DOR: // DOR
      case symbol_kind::S_COMA: // COMA
      case symbol_kind::S_QES: // QES
      case symbol_kind::S_EXM: // EXM
      case symbol_kind::S_ASS: // ASS
      case symbol_kind::S_INTTYPE: // INTTYPE
      case symbol_kind::S_FLOATTYPE: // FLOATTYPE
      case symbol_kind::S_BOOLTYPE: // BOOLTYPE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_VOLATILE: // VOLATILE
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_COL: // COL
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_NEWLINE: // NEWLINE
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_PRINT: // PRINT
      case symbol_kind::S_READ: // READ
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_FOR: // FOR
      case symbol_kind::S_BREAK: // BREAK
      case symbol_kind::S_CONTINUE: // CONTINUE
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_baseType: // baseType
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_program: // program
      case symbol_kind::S_pre_start: // pre_start
      case symbol_kind::S_class: // class
      case symbol_kind::S_classAttrbt: // classAttrbt
      case symbol_kind::S_entry: // entry
      case symbol_kind::S_method: // method
      case symbol_kind::S_arg_method: // arg_method
      case symbol_kind::S_paramList: // paramList
      case symbol_kind::S_type: // type
      case symbol_kind::S_var: // var
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_stmtBl: // stmtBl
      case symbol_kind::S_stmtList: // stmtList
      case symbol_kind::S_forArg1: // forArg1
      case symbol_kind::S_forArg3: // forArg3
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_arg: // arg
      case symbol_kind::S_values: // values
      case symbol_kind::S_postfix: // postfix
      case symbol_kind::S_primary: // primary
        yylhs.value.emplace< Node* > ();
        break;

      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_LCB: // LCB
      case symbol_kind::S_RCB: // RCB
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_RELOP: // RELOP
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_PWR: // PWR
      case symbol_kind::S_LBR: // LBR
      case symbol_kind::S_RBR: // RBR
      case symbol_kind::S_DOR: // DOR
      case symbol_kind::S_COMA: // COMA
      case symbol_kind::S_QES: // QES
      case symbol_kind::S_EXM: // EXM
      case symbol_kind::S_ASS: // ASS
      case symbol_kind::S_INTTYPE: // INTTYPE
      case symbol_kind::S_FLOATTYPE: // FLOATTYPE
      case symbol_kind::S_BOOLTYPE: // BOOLTYPE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_VOLATILE: // VOLATILE
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_COL: // COL
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_NEWLINE: // NEWLINE
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_PRINT: // PRINT
      case symbol_kind::S_READ: // READ
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_FOR: // FOR
      case symbol_kind::S_BREAK: // BREAK
      case symbol_kind::S_CONTINUE: // CONTINUE
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_baseType: // baseType
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // root: program
#line 53 "parser.yy"
              { 
                                root = yystack_[0].value.as < Node* > (); 
}
#line 865 "parser.tab.cc"
    break;

  case 3: // program: stmtEnd pre_start entry "end of file"
#line 58 "parser.yy"
                                {
                                yylhs.value.as < Node* > () = new Node("program","",yylineno);
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 875 "parser.tab.cc"
    break;

  case 4: // program: pre_start entry "end of file"
#line 63 "parser.yy"
                         {
                                yylhs.value.as < Node* > () = new Node("program","",yylineno);
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ()); 
    }
#line 885 "parser.tab.cc"
    break;

  case 5: // pre_start: %empty
#line 71 "parser.yy"
           { 
                                yylhs.value.as < Node* > () = new Node("GlobalList", "", yylineno); 
    }
#line 893 "parser.tab.cc"
    break;

  case 6: // pre_start: pre_start var stmtEnd
#line 74 "parser.yy"
                            { 
                                yylhs.value.as < Node* > () = yystack_[2].value.as < Node* > (); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ()); 
    }
#line 902 "parser.tab.cc"
    break;

  case 7: // pre_start: pre_start class
#line 78 "parser.yy"
                      { 
                                yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > (); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); 
    }
#line 911 "parser.tab.cc"
    break;

  case 8: // class: CLASS ID LCB stmtEnd classAttrbt RCB stmtEnd
#line 86 "parser.yy"
                                                 { 
                                yylhs.value.as < Node* > () = new Node("class", yystack_[5].value.as < std::string > (), yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    }
#line 920 "parser.tab.cc"
    break;

  case 9: // classAttrbt: %empty
#line 93 "parser.yy"
           { 
                                yylhs.value.as < Node* > () = new Node("classAttrbt","",yylineno); 
    }
#line 928 "parser.tab.cc"
    break;

  case 10: // classAttrbt: classAttrbt var stmtEnd
#line 96 "parser.yy"
                                  { 
                                yylhs.value.as < Node* > () = yystack_[2].value.as < Node* > (); yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ()); 
    }
#line 936 "parser.tab.cc"
    break;

  case 11: // classAttrbt: classAttrbt method stmtEnd
#line 99 "parser.yy"
                                     { 
                                yylhs.value.as < Node* > () = yystack_[2].value.as < Node* > (); yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ()); 
    }
#line 944 "parser.tab.cc"
    break;

  case 12: // entry: MAIN LP RP COL INTTYPE stmtBl
#line 105 "parser.yy"
                                  { 
                                yylhs.value.as < Node* > () = new Node("method","main",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(new Node("type", "INT", yylineno));
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 954 "parser.tab.cc"
    break;

  case 13: // method: ID LP arg_method RP COL type stmtBl
#line 113 "parser.yy"
                                        { 
                                yylhs.value.as < Node* > () = new Node("method",yystack_[6].value.as < std::string > (),yylineno);
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[4].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());          
    }
#line 965 "parser.tab.cc"
    break;

  case 14: // arg_method: %empty
#line 122 "parser.yy"
          {
                                yylhs.value.as < Node* > () = new Node("arg_method","",yylineno);
    }
#line 973 "parser.tab.cc"
    break;

  case 15: // arg_method: paramList
#line 125 "parser.yy"
               {
                                yylhs.value.as < Node* > () = new Node("arg_method","",yylineno);
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 982 "parser.tab.cc"
    break;

  case 16: // paramList: ID COL type
#line 132 "parser.yy"
                { 
                                yylhs.value.as < Node* > () = new Node("param",yystack_[2].value.as < std::string > (),yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 991 "parser.tab.cc"
    break;

  case 17: // paramList: paramList COMA ID COL type
#line 136 "parser.yy"
                               { 
                                yylhs.value.as < Node* > () = yystack_[4].value.as < Node* > (); 
                                Node* n = new Node("param",yystack_[2].value.as < std::string > (),yylineno); 
                                n->children.push_back(yystack_[0].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(n);
    }
#line 1002 "parser.tab.cc"
    break;

  case 18: // type: baseType
#line 145 "parser.yy"
             { 
                                yylhs.value.as < Node* > () = new Node("type", yystack_[0].value.as < std::string > (), yylineno); 
    }
#line 1010 "parser.tab.cc"
    break;

  case 19: // type: baseType LBR RBR
#line 148 "parser.yy"
                      { 
                                yylhs.value.as < Node* > () = new Node("type", "arr_"+yystack_[2].value.as < std::string > (), yylineno); 
    }
#line 1018 "parser.tab.cc"
    break;

  case 20: // type: ID
#line 151 "parser.yy"
        { 
                                yylhs.value.as < Node* > () = new Node("type", yystack_[0].value.as < std::string > (), yylineno); 
    }
#line 1026 "parser.tab.cc"
    break;

  case 21: // type: VOID
#line 154 "parser.yy"
          { 
                                yylhs.value.as < Node* > () = new Node("type", "VOID", yylineno); 
    }
#line 1034 "parser.tab.cc"
    break;

  case 22: // var: VOLATILE ID COL type ASS expr
#line 160 "parser.yy"
                                       { 
                                yylhs.value.as < Node* > () = new Node("v_var",yystack_[4].value.as < std::string > (),yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1044 "parser.tab.cc"
    break;

  case 23: // var: ID COL type
#line 165 "parser.yy"
                     { 
                                yylhs.value.as < Node* > () = new Node("var",yystack_[2].value.as < std::string > (),yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1053 "parser.tab.cc"
    break;

  case 24: // var: VOLATILE ID COL type
#line 169 "parser.yy"
                              { 
                                yylhs.value.as < Node* > () = new Node("v_var",yystack_[2].value.as < std::string > (),yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1062 "parser.tab.cc"
    break;

  case 25: // var: ID COL type ASS expr
#line 173 "parser.yy"
                              { 
                                yylhs.value.as < Node* > () = new Node("var",yystack_[4].value.as < std::string > (),yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1072 "parser.tab.cc"
    break;

  case 26: // stmt: stmtBl
#line 181 "parser.yy"
                { 
                                yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); 
     }
#line 1080 "parser.tab.cc"
    break;

  case 27: // stmt: stmtBl stmtEnd
#line 184 "parser.yy"
                        { 
                                yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > (); 
    }
#line 1088 "parser.tab.cc"
    break;

  case 28: // stmt: expr ASS expr stmtEnd
#line 187 "parser.yy"
                               { 
                                yylhs.value.as < Node* > () = new Node("assign","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ()); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1098 "parser.tab.cc"
    break;

  case 29: // stmt: PRINT LP expr RP stmtEnd
#line 192 "parser.yy"
                                  { 
                                yylhs.value.as < Node* > () = new Node("print","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    }
#line 1107 "parser.tab.cc"
    break;

  case 30: // stmt: READ LP expr RP stmtEnd
#line 196 "parser.yy"
                                 { 
                                yylhs.value.as < Node* > () = new Node("read","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    }
#line 1116 "parser.tab.cc"
    break;

  case 31: // stmt: RETURN expr stmtEnd
#line 200 "parser.yy"
                             { 
                                yylhs.value.as < Node* > () = new Node("return","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1125 "parser.tab.cc"
    break;

  case 32: // stmt: CONTINUE stmtEnd
#line 204 "parser.yy"
                                    { yylhs.value.as < Node* > () = new Node("continue","",yylineno); }
#line 1131 "parser.tab.cc"
    break;

  case 33: // stmt: BREAK stmtEnd
#line 205 "parser.yy"
                                    { yylhs.value.as < Node* > () = new Node("break","",yylineno); }
#line 1137 "parser.tab.cc"
    break;

  case 34: // stmt: expr stmtEnd
#line 206 "parser.yy"
                                    { yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > (); }
#line 1143 "parser.tab.cc"
    break;

  case 35: // stmt: var stmtEnd
#line 207 "parser.yy"
                                    { yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > (); }
#line 1149 "parser.tab.cc"
    break;

  case 36: // stmt: IF LP expr RP stmtBl stmtEnd
#line 208 "parser.yy"
                                   {
                                yylhs.value.as < Node* > () = new Node("IF","IF",yylineno);
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1159 "parser.tab.cc"
    break;

  case 37: // stmt: IF LP expr RP stmtBl ELSE stmtBl stmtEnd
#line 213 "parser.yy"
                                                  { 
                                yylhs.value.as < Node* > () = new Node("IF","IFELSE",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[5].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1170 "parser.tab.cc"
    break;

  case 38: // stmt: FOR LP forArg1 COMA expr COMA forArg3 RP stmtBl stmtEnd
#line 219 "parser.yy"
                                                              {
                                yylhs.value.as < Node* > () = new Node("FOR","FOR",yylineno);
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[7].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[5].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1182 "parser.tab.cc"
    break;

  case 39: // stmt: FOR LP forArg1 COMA COMA forArg3 RP stmtBl stmtEnd
#line 226 "parser.yy"
                                                               {
                                yylhs.value.as < Node* > () = new Node("FOR","FOR",yylineno);
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[6].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(new Node("empty arg 2","",yylineno));
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ());
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1194 "parser.tab.cc"
    break;

  case 40: // stmtBl: LCB stmtEnd stmtList RCB
#line 237 "parser.yy"
                             {
                                yylhs.value.as < Node* > () = new Node("Stmtblock","",yylineno);
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1203 "parser.tab.cc"
    break;

  case 41: // stmtList: %empty
#line 245 "parser.yy"
           { 
                                yylhs.value.as < Node* > () = new Node("stmtList","",yylineno); 
    }
#line 1211 "parser.tab.cc"
    break;

  case 42: // stmtList: stmtList stmt
#line 248 "parser.yy"
                    { 
                                yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > (); yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); 
    }
#line 1219 "parser.tab.cc"
    break;

  case 43: // forArg1: %empty
#line 255 "parser.yy"
                                { yylhs.value.as < Node* > () = new Node("for arg 1","",yylineno); }
#line 1225 "parser.tab.cc"
    break;

  case 44: // forArg1: var
#line 256 "parser.yy"
                                { yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); }
#line 1231 "parser.tab.cc"
    break;

  case 45: // forArg1: forArg3
#line 257 "parser.yy"
                                { yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); }
#line 1237 "parser.tab.cc"
    break;

  case 46: // forArg3: expr ASS expr
#line 262 "parser.yy"
                      { 
                                yylhs.value.as < Node* > () = new Node("assign","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1247 "parser.tab.cc"
    break;

  case 49: // expr: primary
#line 276 "parser.yy"
                 { yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); }
#line 1253 "parser.tab.cc"
    break;

  case 50: // expr: expr AND expr
#line 277 "parser.yy"
                       { 
                                yylhs.value.as < Node* > () = new Node("and","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1263 "parser.tab.cc"
    break;

  case 51: // expr: expr OR expr
#line 282 "parser.yy"
                      { 
                                yylhs.value.as < Node* > () = new Node("or","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1273 "parser.tab.cc"
    break;

  case 52: // expr: expr RELOP expr
#line 287 "parser.yy"
                         { 
                                yylhs.value.as < Node* > () = new Node("relop",yystack_[1].value.as < std::string > (),yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1283 "parser.tab.cc"
    break;

  case 53: // expr: expr PLUSOP expr
#line 292 "parser.yy"
                     { 
                                yylhs.value.as < Node* > () = new Node("PLUSOP", "", yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); 
    }
#line 1292 "parser.tab.cc"
    break;

  case 54: // expr: expr MINUSOP expr
#line 296 "parser.yy"
                       { 
                                yylhs.value.as < Node* > () = new Node("MINOP", "", yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); 
    }
#line 1301 "parser.tab.cc"
    break;

  case 55: // expr: expr MULTOP expr
#line 300 "parser.yy"
                      { 
                                yylhs.value.as < Node* > () = new Node("MULTOP", "", yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); 
    }
#line 1310 "parser.tab.cc"
    break;

  case 56: // expr: expr DIV expr
#line 304 "parser.yy"
                   { 
                                yylhs.value.as < Node* > () = new Node("DIVOP", "", yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); 
    }
#line 1319 "parser.tab.cc"
    break;

  case 57: // expr: expr PWR expr
#line 308 "parser.yy"
                   { 
                                yylhs.value.as < Node* > () = new Node("PWROP", "Power", yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ()); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); 
    }
#line 1329 "parser.tab.cc"
    break;

  case 58: // expr: expr postfix
#line 313 "parser.yy"
                      { 
                                yylhs.value.as < Node* > () = new Node("postfix","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ()); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1339 "parser.tab.cc"
    break;

  case 59: // expr: baseType LBR values RBR
#line 318 "parser.yy"
                                 { 
                                yylhs.value.as < Node* > () = new Node("init_array","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(new Node("type", yystack_[3].value.as < std::string > (), yylineno));
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ()); 
    }
#line 1349 "parser.tab.cc"
    break;

  case 60: // expr: EXM expr
#line 323 "parser.yy"
                  { 
                                yylhs.value.as < Node* > () = new Node("not","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1358 "parser.tab.cc"
    break;

  case 61: // expr: ID LP arg RP
#line 327 "parser.yy"
                      { 
                                yylhs.value.as < Node* > () = new Node("call",yystack_[3].value.as < std::string > (),yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1367 "parser.tab.cc"
    break;

  case 62: // arg: %empty
#line 334 "parser.yy"
           { 
                                yylhs.value.as < Node* > () = new Node("args", "", yylineno); 
    }
#line 1375 "parser.tab.cc"
    break;

  case 63: // arg: expr
#line 337 "parser.yy"
         { 
                                yylhs.value.as < Node* > () = new Node("args", "", yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); 
    }
#line 1384 "parser.tab.cc"
    break;

  case 64: // arg: arg COMA expr
#line 341 "parser.yy"
                  { 
                                yylhs.value.as < Node* > () = yystack_[2].value.as < Node* > (); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); 
    }
#line 1393 "parser.tab.cc"
    break;

  case 65: // values: expr
#line 349 "parser.yy"
    {
      yylhs.value.as < Node* > () = new Node("values","",yylineno);
      yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1402 "parser.tab.cc"
    break;

  case 66: // values: values COMA expr
#line 354 "parser.yy"
    {
      yylhs.value.as < Node* > () = yystack_[2].value.as < Node* > ();
      yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1411 "parser.tab.cc"
    break;

  case 67: // baseType: INTTYPE
#line 360 "parser.yy"
                                    { yylhs.value.as < std::string > () = "INT"; }
#line 1417 "parser.tab.cc"
    break;

  case 68: // baseType: FLOATTYPE
#line 361 "parser.yy"
                                    { yylhs.value.as < std::string > () = "FLOAT"; }
#line 1423 "parser.tab.cc"
    break;

  case 69: // baseType: BOOLTYPE
#line 362 "parser.yy"
                                    { yylhs.value.as < std::string > () = "BOOL"; }
#line 1429 "parser.tab.cc"
    break;

  case 70: // postfix: LBR expr RBR
#line 368 "parser.yy"
                     { 
                                yylhs.value.as < Node* > () = new Node("arr_access","",yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ()); 
    }
#line 1438 "parser.tab.cc"
    break;

  case 71: // postfix: DOT LENGTH
#line 372 "parser.yy"
                    {               
                                yylhs.value.as < Node* > () = new Node("length","",yylineno); 
    }
#line 1446 "parser.tab.cc"
    break;

  case 72: // postfix: DOT ID LP arg RP
#line 375 "parser.yy"
                          { 
                                yylhs.value.as < Node* > () = new Node("funcMethcall",yystack_[3].value.as < std::string > (),yylineno); 
                                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1455 "parser.tab.cc"
    break;

  case 73: // primary: INT
#line 382 "parser.yy"
                                    { yylhs.value.as < Node* > () = new Node("INT",yystack_[0].value.as < std::string > (),yylineno); }
#line 1461 "parser.tab.cc"
    break;

  case 74: // primary: FLOAT
#line 383 "parser.yy"
                                    { yylhs.value.as < Node* > () = new Node("FLOAT",yystack_[0].value.as < std::string > (),yylineno); }
#line 1467 "parser.tab.cc"
    break;

  case 75: // primary: BOOL
#line 384 "parser.yy"
                                    { yylhs.value.as < Node* > () = new Node("BOOL",yystack_[0].value.as < std::string > (),yylineno); }
#line 1473 "parser.tab.cc"
    break;

  case 76: // primary: LP expr RP
#line 385 "parser.yy"
                                    { yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > (); }
#line 1479 "parser.tab.cc"
    break;

  case 77: // primary: ID
#line 386 "parser.yy"
                                    { yylhs.value.as < Node* > () = new Node("ID",yystack_[0].value.as < std::string > (),yylineno); }
#line 1485 "parser.tab.cc"
    break;


#line 1489 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short parser::yypact_ninf_ = -155;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -21,  -155,    27,  -155,    -7,    14,  -155,    30,    47,    57,
      71,  -155,    83,   -21,  -155,    -7,    62,    51,    74,    80,
    -155,    14,    86,  -155,  -155,  -155,  -155,  -155,    68,    76,
      62,    63,   -21,  -155,   185,    77,    73,    79,    14,  -155,
     101,   185,   185,  -155,  -155,   379,    93,  -155,  -155,   185,
     102,    -3,   185,   260,   -10,   185,   185,   185,   185,   185,
     185,   185,   185,   185,     8,  -155,   185,   379,   -21,  -155,
      11,   -21,   -21,   -21,   379,    35,  -155,   184,   184,    23,
     113,   396,   220,    23,    23,   277,  -155,   103,   379,    56,
      14,   107,    14,    14,    14,  -155,   185,  -155,   185,  -155,
     185,   137,    85,   110,   104,   379,    39,   379,    12,  -155,
     114,   123,   125,   185,   126,   -21,   -21,   -21,  -155,   -21,
     216,    62,    90,   131,  -155,   185,   185,   185,   238,    38,
      14,    14,    14,    14,   185,    14,  -155,    62,   108,   294,
     311,   328,    14,  -155,   122,  -155,   345,   238,   102,    62,
     102,   -21,   -21,   161,   185,    14,  -155,  -155,     4,    14,
      14,   185,   362,   379,   102,    14,   140,   185,   -21,   102,
     141,    14,   -21,   102,    14,   -21,    14
  };

  const signed char
  parser::yydefact_[] =
  {
       5,    47,     0,     2,     0,     5,     1,     0,     0,     0,
       0,     7,     0,     0,    48,     0,     0,     0,     0,     0,
       4,     6,     0,    20,    67,    68,    69,    21,    23,    18,
       0,     0,     0,     3,     0,     0,    24,     0,     9,    73,
      77,     0,     0,    75,    74,    25,     0,    49,    19,     0,
       0,     0,    62,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    22,     0,    12,
       0,     0,     0,     0,    63,     0,    76,    53,    54,    55,
      50,    51,    52,    56,    57,     0,    71,     0,    65,     0,
      41,    14,     8,    11,    10,    61,     0,    70,    62,    59,
       0,     0,     0,     0,    15,    64,     0,    66,    77,    40,
       0,     0,     0,     0,     0,     0,     0,     0,    42,    26,
       0,     0,     0,     0,    72,     0,     0,     0,     0,    43,
      33,    32,    35,    27,     0,    34,    16,     0,     0,     0,
       0,     0,    31,    44,     0,    45,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28,    13,    17,     0,    29,
      30,     0,     0,    46,     0,    36,     0,     0,     0,     0,
       0,    37,     0,     0,    39,     0,    38
  };

  const short
  parser::yypgoto_[] =
  {
    -155,  -155,  -155,   143,  -155,  -155,   138,  -155,  -155,  -155,
     -27,   -50,  -155,   -96,  -155,  -155,  -154,   -13,   -26,    58,
    -155,   -12,  -155,  -155
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     2,     3,     4,    11,    51,    12,    72,   103,   104,
      28,    13,   118,    69,   101,   144,   145,     5,   146,    75,
      89,    46,    65,    47
  };

  const unsigned char
  parser::yytable_[] =
  {
      21,    73,     7,    36,    29,   119,    70,   166,    45,    63,
      71,    86,   164,   170,     1,    53,    54,    87,    29,    38,
      64,    91,    52,    67,     8,     9,    74,     6,     8,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    10,     1,
      88,    62,    63,    39,    16,    16,    95,   108,    41,    14,
     124,   117,   156,    64,   158,    90,    17,    96,    92,    93,
      94,    96,    42,    16,    24,    25,    26,    18,   168,     8,
     105,    23,    74,   172,   107,   120,    99,   175,   100,   143,
      19,    43,    44,    20,    30,    31,    33,   128,    24,    25,
      26,    27,    32,    34,   136,    35,    37,    48,    49,   139,
     140,   141,   130,   131,   132,    50,   133,   135,   147,    29,
     148,    52,    66,    98,    68,   142,   102,    55,   121,    56,
      57,   122,   157,   137,   125,    29,   123,   162,   163,    60,
      61,    62,    63,   126,   155,   127,   129,    29,   159,   160,
     138,   149,    39,    64,   153,   165,   108,    41,    15,    68,
     109,   169,   173,    22,     0,   171,   106,     0,     0,   174,
       0,    42,   176,    24,    25,    26,    39,     0,     8,     0,
      40,    41,     0,   110,   111,   112,   113,   114,   115,   116,
      43,    44,     0,   161,     0,    42,     0,    24,    25,    26,
      39,    57,     0,     0,    40,    41,     0,     0,     0,     0,
       0,    61,    62,    63,    43,    44,     0,     0,     0,    42,
       0,    24,    25,    26,    64,     0,     0,     0,     0,     0,
      55,     0,    56,    57,    55,     0,    56,    57,    43,    44,
      58,    59,    60,    61,    62,    63,     0,    61,    62,    63,
       0,   134,    55,     0,    56,    57,    64,     0,     0,     0,
      64,     1,    58,    59,    60,    61,    62,    63,     0,     0,
       0,     0,     0,     0,    55,     0,    56,    57,    64,     0,
       0,    76,     0,     1,    58,    59,    60,    61,    62,    63,
       0,    55,     0,    56,    57,     0,     0,     0,     0,     0,
      64,    58,    59,    60,    61,    62,    63,    97,    55,     0,
      56,    57,     0,     0,     0,   150,     0,    64,    58,    59,
      60,    61,    62,    63,     0,    55,     0,    56,    57,     0,
       0,     0,   151,     0,    64,    58,    59,    60,    61,    62,
      63,     0,    55,     0,    56,    57,     0,     0,     0,   152,
       0,    64,    58,    59,    60,    61,    62,    63,     0,    55,
       0,    56,    57,     0,     0,     0,     0,     0,    64,    58,
      59,    60,    61,    62,    63,     0,    55,     0,    56,    57,
     154,     0,     0,     0,     0,    64,    58,    59,    60,    61,
      62,    63,     0,    55,   167,    56,    57,     0,     0,     0,
       0,     0,    64,    58,    59,    60,    61,    62,    63,     0,
      55,     0,    56,    57,     0,     0,     0,     0,     0,    64,
      58,     0,    60,    61,    62,    63,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64
  };

  const short
  parser::yycheck_[] =
  {
      13,    51,     9,    30,    16,   101,     9,   161,    34,    19,
      13,     3,     8,   167,    35,    41,    42,     9,    30,    32,
      30,    10,    10,    49,    31,    32,    52,     0,    31,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    45,    35,
      66,    18,    19,     5,    33,    33,    11,     9,    10,    35,
      11,   101,   148,    30,   150,    68,     9,    22,    71,    72,
      73,    22,    24,    33,    26,    27,    28,    10,   164,    31,
      96,     9,    98,   169,   100,   101,    20,   173,    22,   129,
       9,    43,    44,     0,    33,    11,     0,   113,    26,    27,
      28,    29,    12,    25,   121,    19,    33,    20,    25,   125,
     126,   127,   115,   116,   117,    26,   119,   120,   134,   121,
     137,    10,    19,    10,    12,   128,     9,     4,    33,     6,
       7,    11,   149,    33,    10,   137,    22,   153,   154,    16,
      17,    18,    19,    10,   147,    10,    10,   149,   151,   152,
       9,    33,     5,    30,    22,   158,     9,    10,     5,    12,
      13,    11,    11,    15,    -1,   168,    98,    -1,    -1,   172,
      -1,    24,   175,    26,    27,    28,     5,    -1,    31,    -1,
       9,    10,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    22,    -1,    24,    -1,    26,    27,    28,
       5,     7,    -1,    -1,     9,    10,    -1,    -1,    -1,    -1,
      -1,    17,    18,    19,    43,    44,    -1,    -1,    -1,    24,
      -1,    26,    27,    28,    30,    -1,    -1,    -1,    -1,    -1,
       4,    -1,     6,     7,     4,    -1,     6,     7,    43,    44,
      14,    15,    16,    17,    18,    19,    -1,    17,    18,    19,
      -1,    25,     4,    -1,     6,     7,    30,    -1,    -1,    -1,
      30,    35,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,     4,    -1,     6,     7,    30,    -1,
      -1,    11,    -1,    35,    14,    15,    16,    17,    18,    19,
      -1,     4,    -1,     6,     7,    -1,    -1,    -1,    -1,    -1,
      30,    14,    15,    16,    17,    18,    19,    20,     4,    -1,
       6,     7,    -1,    -1,    -1,    11,    -1,    30,    14,    15,
      16,    17,    18,    19,    -1,     4,    -1,     6,     7,    -1,
      -1,    -1,    11,    -1,    30,    14,    15,    16,    17,    18,
      19,    -1,     4,    -1,     6,     7,    -1,    -1,    -1,    11,
      -1,    30,    14,    15,    16,    17,    18,    19,    -1,     4,
      -1,     6,     7,    -1,    -1,    -1,    -1,    -1,    30,    14,
      15,    16,    17,    18,    19,    -1,     4,    -1,     6,     7,
      25,    -1,    -1,    -1,    -1,    30,    14,    15,    16,    17,
      18,    19,    -1,     4,    22,     6,     7,    -1,    -1,    -1,
      -1,    -1,    30,    14,    15,    16,    17,    18,    19,    -1,
       4,    -1,     6,     7,    -1,    -1,    -1,    -1,    -1,    30,
      14,    -1,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30
  };

  const signed char
  parser::yystos_[] =
  {
       0,    35,    47,    48,    49,    63,     0,     9,    31,    32,
      45,    50,    52,    57,    35,    49,    33,     9,    10,     9,
       0,    63,    52,     9,    26,    27,    28,    29,    56,    67,
      33,    11,    12,     0,    25,    19,    56,    33,    63,     5,
       9,    10,    24,    43,    44,    64,    67,    69,    20,    25,
      26,    51,    10,    64,    64,     4,     6,     7,    14,    15,
      16,    17,    18,    19,    30,    68,    19,    64,    12,    59,
       9,    13,    53,    57,    64,    65,    11,    64,    64,    64,
      64,    64,    64,    64,    64,    64,     3,     9,    64,    66,
      63,    10,    63,    63,    63,    11,    22,    20,    10,    20,
      22,    60,     9,    54,    55,    64,    65,    64,     9,    13,
      36,    37,    38,    39,    40,    41,    42,    57,    58,    59,
      64,    33,    11,    22,    11,    10,    10,    10,    64,    10,
      63,    63,    63,    63,    25,    63,    56,    33,     9,    64,
      64,    64,    63,    57,    61,    62,    64,    64,    56,    33,
      11,    11,    11,    22,    25,    63,    59,    56,    59,    63,
      63,    22,    64,    64,     8,    63,    62,    22,    59,    11,
      62,    63,    59,    11,    63,    59,    63
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    46,    47,    48,    48,    49,    49,    49,    50,    51,
      51,    51,    52,    53,    54,    54,    55,    55,    56,    56,
      56,    56,    57,    57,    57,    57,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      59,    60,    60,    61,    61,    61,    62,    63,    63,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    65,    65,    65,    66,    66,    67,    67,    67,
      68,    68,    68,    69,    69,    69,    69,    69
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     4,     3,     0,     3,     2,     7,     0,
       3,     3,     6,     7,     0,     1,     3,     5,     1,     3,
       1,     1,     6,     3,     4,     5,     1,     2,     4,     5,
       5,     3,     2,     2,     2,     2,     6,     8,    10,     9,
       4,     0,     2,     0,     1,     1,     3,     1,     2,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     4,
       2,     4,     0,     1,     3,     1,     3,     1,     1,     1,
       3,     2,     5,     1,     1,     1,     3,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "LENGTH", "PLUSOP",
  "INT", "MINUSOP", "MULTOP", "ELSE", "ID", "LP", "RP", "LCB", "RCB",
  "AND", "OR", "RELOP", "DIV", "PWR", "LBR", "RBR", "DOR", "COMA", "QES",
  "EXM", "ASS", "INTTYPE", "FLOATTYPE", "BOOLTYPE", "VOID", "DOT",
  "VOLATILE", "MAIN", "COL", "SEMICOL", "NEWLINE", "IF", "PRINT", "READ",
  "RETURN", "FOR", "BREAK", "CONTINUE", "BOOL", "FLOAT", "CLASS",
  "$accept", "root", "program", "pre_start", "class", "classAttrbt",
  "entry", "method", "arg_method", "paramList", "type", "var", "stmt",
  "stmtBl", "stmtList", "forArg1", "forArg3", "stmtEnd", "expr", "arg",
  "values", "baseType", "postfix", "primary", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    53,    53,    58,    63,    71,    74,    78,    86,    93,
      96,    99,   105,   113,   122,   125,   132,   136,   145,   148,
     151,   154,   160,   165,   169,   173,   181,   184,   187,   192,
     196,   200,   204,   205,   206,   207,   208,   213,   219,   226,
     237,   245,   248,   255,   256,   257,   262,   270,   271,   276,
     277,   282,   287,   292,   296,   300,   304,   308,   313,   318,
     323,   327,   334,   337,   341,   348,   353,   360,   361,   362,
     368,   372,   375,   382,   383,   384,   385,   386
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 2112 "parser.tab.cc"

