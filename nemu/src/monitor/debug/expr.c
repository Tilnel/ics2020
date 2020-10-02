#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <stdlib.h>
#include <memory/paddr.h>
enum {
  TK_NOTYPE = 256, TK_EQ = 255, TK_NUM = 254, TK_PLUS = '+', TK_SUBS = '-', TK_TIMES = '*', TK_DIVIDE = '/', TK_LPAREN = '(', TK_RPAREN = ')', TK_REG = 253, TK_HEX = 252, TK_DEREF = 251,  

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {"0x[a-f0-9]+", TK_HEX}, // hexademical num
  {"\\$[a-z]+", TK_REG},// registers
  {" +", TK_NOTYPE},    // spaces
  {"\\-", '-'},		// subs
  {"\\+", '+'},         // plus
  {"\\*", '*'},		// times, dereference
  {"\\/", '/'},		// divide
  {"\\(", '('},		// open paren
  {"\\)", ')'},		// close paren
  {"[0-9]+", TK_NUM},	// demical number
  {"==", TK_EQ},        // equal
  
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[8192] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i, len;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    uint32_t num = 0;
    bool flag;
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NUM : {
	    tokens[nr_token].type = TK_NUM;
	    strncpy(tokens[nr_token].str, e + position - substr_len, substr_len);
	    tokens[nr_token].str[substr_len]='\0';
	    nr_token++;
	    break;
	  }
	  case TK_HEX : 
	    tokens[nr_token].type = TK_NUM;
	    num = (uint32_t)strtol(e + position - substr_len, NULL, 0);
	    len = sprintf(tokens[nr_token].str, "%u\n", num);
	    tokens[nr_token].str[len]='\0';
	    nr_token++;
	    break;
          case TK_REG :
	    tokens[nr_token].type = TK_NUM;
	    num = isa_reg_str2val(e + position - substr_len + 1, &flag);
	    len = sprintf(tokens[nr_token].str, "%u\n", num);
	    tokens[nr_token].str[len]='\0';
	    //printf("%s\n",tokens[nr_token].str);
	    nr_token++;
	    break;
	  case TK_NOTYPE : break;
	  case '*' : 
	    if (nr_token == 0 || (tokens[nr_token - 1].type != TK_NUM &&
			    tokens[nr_token - 1].type != ')') ||
			    tokens[nr_token - 1].type == '(')
	      tokens[nr_token].type = TK_DEREF;
	    else tokens[nr_token].type = '*';
	    nr_token ++;
	    break;
          default: //TODO();
	    tokens[nr_token].type = rules[i].token_type;
            nr_token++;
	}
 	

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(int p, int q){
  int paren = 0;
  if (tokens[p].type != '(' || tokens[q].type != ')') return false;
  for (int i = p; i < q; i++) {
    if (tokens[i].type == '(') paren++;
    if (tokens[i].type == ')') paren--;
    if (paren == 0) return false;
  }
  return true;
}
    

word_t eval (int p, int q, bool *flag) {
  int op = 0, op_type = 0, paren, type;
  if (p > q) {
    *flag = false;
    return 0;
  }
  else if (p == q) {
    return (word_t)strtol(tokens[p].str, NULL, 10);
  }
  else if (tokens[p].type == TK_DEREF) {
    word_t val1 = eval(p + 1, q, flag);
    return paddr_read(val1, 4);
  }
  else if (check_parentheses(p, q) == true) {
    return eval(p + 1, q - 1, flag);
  }
  else { 
    paren = 0;
    for (int i = p; i <= q; i++) {
      type = tokens[i].type;
      if (type == '(') paren++;
      if (type == ')') paren--;
      if (paren == 0) {
	if (op_type != '+' && op_type != '-' && (type == '*' || type == '/')) {op_type = type; op = i;}
	if (type == '+' || type == '-') {op_type = type; op = i;}
      }
    }
    word_t val1 = eval(p, op - 1, flag);
    word_t val2 = eval(op + 1, q, flag);
    switch (op_type){
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': {
	if (val2 == 0) {
	  *flag = false;
	  return 0;
	}
	return val1 / val2;
      }
      default: *flag = false;
    }
  }
  return 0;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
 // TODO();
  *success = true;
  word_t result = 0;
  bool flag = true;
  //Log("%d tokens left\n", nr_token);
  result = eval(0, nr_token-1, &flag);
  if (flag) return result;
  else {
    *success = false;
    printf("Bad expression!\n");
  }
  return 0;
}
