#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
int lineno = 1;
//****************************************** TOKEN NUMBER ************************************
const int IF = 0;
const int ELSE = 1;
const int ELIF = 2;
const int IN = 3;
const int RANGE = 4;
const int FOR = 5;
const int WHILE = 6;
const int DEF = 7;
const int INT = 8;
const int FLOAT = 9;
const int STR = 10;
const int BOOL = 11;
const int INDEN = 12;
const int INPUT = 13;
const int LPAR = 14;
const int RPAR = 15;
const int PRINT = 16;
const int AND = 17;
const int NOT = 18;
const int OR = 19;
const int LE = 20;
const int GE = 21;
const int NE = 22;
const int EQ = 23;
const int LT = 24;
const int GT = 25;
const int ADD = 26;
const int SUB = 27;
const int DIV = 28;
const int MUL = 29;
const int DOT = 30;
const int COMMA = 31;
const int COLON = 32;
const int TRUE = 33;
const int FALSE = 34;
const int NUM = 35;
const int IDEN = 36;
const int ASSIGN = 37;
const int EOFT=38;
int token_cnt = 0;
int is_lexical_error=0;
int is_paranthesis_error= 0;
char* INPUT_FILE = "input.txt";
char* append_char_to_str(char* str, char c){
    size_t len = strlen(str);
   
    /* one for extra char, one for trailing zero */
    char *str2 = malloc(len + 1 + 1);

    strcpy(str2, str);
    str2[len] = c;
    str2[len + 1] = '\0';

    return str2;
}
char* file_to_str(){
    FILE    *textfile;
    char    *text;
    long    numbytes;
     
    textfile = fopen(INPUT_FILE, "r");
    fseek(textfile, 0L, SEEK_END);
    numbytes = ftell(textfile);
    fseek(textfile, 0L, SEEK_SET);  
    text = (char*)calloc(numbytes, sizeof(char));   
    fread(text, sizeof(char), numbytes, textfile);
    fclose(textfile); 
 
    return text;
}
int is_indentation_error(char* data_str){
    int prev_indentation_no = 0;
    int iscolon = 0;
    int data_len = strlen(data_str);
    for(int i = 0; i < data_len;){
        if(data_str[i] == ':'){
            iscolon = 1;
        }
        else if(data_str[i] != ' ' && data_str[i] != '\n'){
            iscolon = 0;
        }
        if(data_str[i] == '\n'){
            lineno++;
        }
        if(data_str[i] == '#'){
            while(i < data_len && data_str[i] != '\n'){
                i++;
            }
            continue;
        }
        if(data_str[i] == '\n'){
            i++;
            int curr_indentation_no = 0;
            while(i < data_len && data_str[i] == ' '){
                i++;
                curr_indentation_no++;
            }
            if(i < data_len && data_str[i] == '\n'){
                continue;
            }
            if(curr_indentation_no%4 != 0){
                return lineno;
            }
            curr_indentation_no /= 4;
            if(iscolon == 1 && curr_indentation_no != prev_indentation_no + 1){
                return lineno;
            }
            else if(iscolon == 0 && curr_indentation_no > prev_indentation_no){
                return lineno;
            }
            prev_indentation_no = curr_indentation_no;
        }
        else{
            i++;
        }
    }
    // printf(data_str);
    return 0;
}
struct token{
    char* lexeme;
    int token_type; 
    int indent_no;
};
int get_tokens_cnt(char* data_str){
    int cnt = 0;int line_no=1;
    int data_len = strlen(data_str);
    for(int i = 0 ; i < data_len;){
        // printf("%c\n",data_str[i]);
        //************************** COMMENT *************************************
        if(data_str[i] == '#'){
            while(i < data_len && data_str[i] != '\n'){
                i++;
            }
            continue;
        }
        //************************** STRING **************************************
        if(data_str[i] == '\''){
            char* str = "";
            i++;
            while (i < data_len && (data_str[i - 1] == '\\' || data_str[i] != '\'')){
                str = append_char_to_str(str,data_str[i]);
                i++;
            }
            cnt++;
            continue;
        }
        if(data_str[i] == '\"'){
            char* str = "";
            i++;
            while (i < data_len && (data_str[i - 1] == '\\' || data_str[i] != '\"')){
                str = append_char_to_str(str,data_str[i]);
                i++;
            }
            cnt++;
            continue;
        }
        //************************** GAPS ****************************************
        if(data_str[i] == ' '){
            i++;
            continue;
        }
        //***************************INDENTATION************************************
        if(data_str[i] == '\n'){
            int curr_inden = 0;line_no++;
            while(i < data_len && (data_str[i] == '\n' || data_str[i] == ' ')){
                if(data_str[i] == '\n'){
                    curr_inden = 0;
                }
                else{
                    curr_inden++;
                }
                i++;
            }
            cnt++;
            continue;
        }
        //******************************FOR******************************************
        if(i + 3 < data_len && data_str[i] == 'f' && data_str[i + 1] == 'o' && data_str[i+2] == 'r' && (data_str[i + 3] == ' ')){
            cnt++;
            i += 3;
            continue;
        }
        //******************************WHILE******************************************
        if(i + 5 < data_len && data_str[i] == 'w' && data_str[i + 1] == 'h' && data_str[i+2] == 'i' && data_str[i+3] == 'l'&& data_str[i+4] == 'e'&& (data_str[i + 5] == ' '||data_str[i + 5] == '(')){
            cnt++;
            i += 5;
            continue;
        }
        //*****************************IN KEYWORD*************************************
        if(i + 2 < data_len && data_str[i] == 'i' && data_str[i + 1] == 'n' && (data_str[i + 2] == ' ')){
            cnt++;
            i += 2;
            continue;
        }
        //*****************************IF KEYWORD*************************************
        if(i + 2 < data_len && data_str[i] == 'i' && data_str[i + 1] == 'f' && (data_str[i + 2] == ' ' || data_str[i + 2] == '(')){
            cnt++;
            i += 2;
            continue;
        }
        //*****************************ELSE KEYWORD*************************************
        if(i + 4 < data_len && data_str[i] == 'e' && data_str[i + 1] == 'l' && data_str[i + 2] == 's' && data_str[i + 3] == 'e' && (data_str[i + 4] == ' ' || data_str[i + 4] == ':') ){
            cnt++;
            i += 4;
            continue;
        }
        //*****************************ELIF KEYWORD*************************************
        if(i + 4 < data_len && data_str[i] == 'e' && data_str[i + 1] == 'l' && data_str[i + 2] == 'i' && data_str[i + 3] == 'f' && (data_str[i + 4] == ' ' || data_str[i + 4] == '(') ){
            cnt++;
            i += 4;
            continue;
        }
        //*****************************RANGE KEYWORD*************************************
        if(i + 5 < data_len && data_str[i] == 'r' && data_str[i + 1] == 'a' && data_str[i + 2] == 'n' && data_str[i + 3] == 'g' && data_str[i + 4] == 'e' && data_str[i + 5] == '('){
            cnt++;
            i += 5;
            continue;
        }
        //*****************************INPUT KEYWORD*************************************
        if(i+5<data_len&&data_str[i]=='i'&&data_str[i+1]=='n'&&data_str[i+2]=='p'&&data_str[i+3]=='u'&&data_str[i+4]=='t' && data_str[i + 5] == '('){
            cnt++;
            i=i+5;
            continue;
        }
        //*****************************LPAR KEYWORD*************************************
        if(data_str[i]=='('){
            cnt++;
            i=i+1;
            continue;
        }
        //*****************************RPAR KEYWORD*************************************
        if(data_str[i]==')'){
            cnt++;
            i=i+1;
            continue;
        }
        //*****************************PRINT KEYWORD*************************************
        if(i+4<data_len&&data_str[i]=='p'&&data_str[i+1]=='r'&&data_str[i+2]=='i'&&data_str[i+3]=='n'&&data_str[i+4]=='t'){
            cnt++;
            i=i+5;
            continue;
        }
        //*****************************AND KEYWORD*************************************
        if(i+3<data_len&&data_str[i]=='a'&&data_str[i+1]=='n'&&data_str[i+2]=='d'&&data_str[i+3]==' '){
            cnt++;
            i=i+4;
            continue;
        }
        //*****************************NOT KEYWORD*************************************
        if(i+3<data_len&&data_str[i]=='n'&&data_str[i+1]=='o'&&data_str[i+2]=='t'&&data_str[i+3]==' '){
            cnt++;
            i=i+4;
            continue;
        }
        //*****************************OR KEYWORD*************************************
        if(i+2<data_len&&data_str[i]=='o'&&data_str[i+1]=='r'&&data_str[i+2]==' '){
            cnt++;
            i=i+4;
            continue;
        }
        //*****************************ASSIGN**************************************************
        if(i + 2 < data_len && data_str[i] == '+' && data_str[i + 1] == '='  ){
            cnt++;
            i += 2;
            continue;
        }
        if(i + 2 < data_len && data_str[i] == '-' && data_str[i + 1] == '='  ){
            cnt++;
            i += 2;
            continue;
        }
        if(i + 2 < data_len && data_str[i] == '/' && data_str[i + 1] == '='  ){
            cnt++;
            i += 2;
            continue;
        }
        if(i + 2 < data_len && data_str[i] == '*' && data_str[i + 1] == '='  ){
            cnt++;
            i += 2;
            continue;
        }
        //*****************************ARITHMETIC OPERATORS*************************************
        char arithmetics[7] = {'+', '-', '/', '*', ':', '.', ','};
        int arithmetic_token_no[7] = {ADD, SUB, DIV, MUL, COLON, DOT, COMMA};
        int is_arithmetic = 0;
        for(int k = 0 ;k < 7; k++){
            if(arithmetics[k] == data_str[i]){
                is_arithmetic = 1;
                char* new_str = "";
                new_str = append_char_to_str(new_str,arithmetics[k]);
                i++;
                cnt++;
                break;
            }
        }
        if(is_arithmetic == 1){
            continue;
        }
        //**********************************RELOP*************************************
        if(i + 2 < data_len && data_str[i] == '<' && data_str[i + 1] == '='  ){
            cnt++;
            i += 2;
            continue;
        }

        if(i + 2 < data_len && data_str[i] == '>' && data_str[i + 1] == '='  ){
            cnt++;
            i += 2;
            continue;
        }

        if(i + 2 < data_len && data_str[i] == '=' && data_str[i + 1] == '='  ){
            cnt++;
            i += 2;
            continue;
        }

        if(i + 2 < data_len && data_str[i] == '!' && data_str[i + 1] == '='  ){
            cnt++;
            i += 2;
            continue;
        }
        

        if(i + 1 < data_len && data_str[i] == '<'   ){
            cnt++;
            i += 1;
            continue;
        }

        if(i + 1 < data_len && data_str[i] == '>'  ){
            cnt++;
            i += 1;
            continue;
        }
        if(i + 1 < data_len && data_str[i] == '='  ){
            cnt++;
            i += 1;
            continue;
        }
        //*******************************BOOL*****************************************
        if(i+3<data_len&& data_str[i] == 'T' && data_str[i + 1] == 'r' && data_str[i + 2] == 'u' && data_str[i + 3] == 'e'&&(i+4==data_len||data_str[i+4]==' '||data_str[i+4]=='\n')  ){
            cnt++;
            i += 4;
            continue;
        }

        
        if(i+4<data_len&& data_str[i] == 'F' && data_str[i + 1] == 'a' && data_str[i + 2] == 'l' && data_str[i + 3] == 's'&&data_str[i + 4] == 'e'&&(i+5==data_len||data_str[i+5]==' '||data_str[i+5]=='\n')){
            cnt++;
            i += 5;
            continue;
        }
        //********************************NUM*****************************************
        if((data_str[i]>='0'&&data_str[i]<='9')){
            char* s1 = "";int t=i;
            while(t < data_len && (data_str[t]>='0'&&data_str[t]<='9')){
                s1 = append_char_to_str(s1,data_str[t]);
                t++;
            }
            cnt++;
            i=t;
            continue;
        }
        //*****************************ID KEYWORD*************************************
        if((data_str[i]>='a'&&data_str[i]<='z')||(data_str[i]>='A'&&data_str[i]<='Z')||data_str[i]=='_'){
            char* s1 = "";int t=i;
            while(t<data_len&&((data_str[t]>='a'&&data_str[t]<='z')||(data_str[t]>='A'&&data_str[t]<='Z')||data_str[t]=='_'||(data_str[t]>='0'&&data_str[t]<='9'))){
                s1 = append_char_to_str(s1,data_str[t]);
                // printf("Error detected");
                t++;
            }
            cnt++;
            i=t;
            continue;
        }
        is_lexical_error=1;
        return line_no;
    }
    return cnt;
}
void get_tokens(char* data_str, struct token* tokens){
    int j = 0;
    int data_len = strlen(data_str);
    for(int i = 0 ; i < data_len;){
        //************************** COMMENT *************************************
        if(data_str[i] == '#'){
            while(i < data_len && data_str[i] != '\n'){
                i++;
            }
            continue;
        }
        //************************** STRING **************************************
        if(data_str[i] == '\''){
            char* str = "";
            i++;
            while (i < data_len && (data_str[i - 1] == '\\' || data_str[i] != '\'')){
                str = append_char_to_str(str,data_str[i]);
                i++;
            }
            struct token temp;
            temp.lexeme = str;
            temp.token_type = STR;
            
            tokens[j] = temp;
j++;
            continue;
        }
        if(data_str[i] == '\"'){
            char* str = "";
            i++;
            while (i < data_len && (data_str[i - 1] == '\\' || data_str[i] != '\"')){
                str = append_char_to_str(str,data_str[i]);
                i++;
            }
            struct token temp;
            temp.lexeme = str;
            temp.token_type = STR;
            tokens[j] = temp;
j++;
            continue;
        }
        //************************** GAPS ****************************************
        if(data_str[i] == ' '){
            i++;
            continue;
        }
        //***************************INDENTATION************************************
        if(data_str[i] == '\n'){
            int curr_inden = 0;
            while(i < data_len && (data_str[i] == '\n' || data_str[i] == ' ')){
                if(data_str[i] == '\n'){
                    curr_inden = 0;
                }
                else{
                    curr_inden++;
                }
                i++;
            }
            struct token temp;
            temp.indent_no = curr_inden/4;
            temp.lexeme = "Indent";
            temp.token_type = INDEN;
            tokens[j] = temp;
j++;
            continue;
        }
        //******************************FOR******************************************
        if(i + 3 < data_len && data_str[i] == 'f' && data_str[i + 1] == 'o' && data_str[i+2] == 'r' && (data_str[i + 3] == ' ')){
            struct token temp;
            temp.token_type = FOR;
            temp.lexeme = "for";
            tokens[j] = temp;
j++;
            i += 3;
            continue;
        }
        //******************************WHILE******************************************
        if(i + 5 < data_len && data_str[i] == 'w' && data_str[i + 1] == 'h' && data_str[i+2] == 'i' && data_str[i+3] == 'l'&& data_str[i+4] == 'e'&& (data_str[i + 5] == ' '||data_str[i + 5] == '(')){
            struct token temp;
            temp.token_type = WHILE;
            temp.lexeme = "while";
            tokens[j] = temp;
j++;
            i += 5;
            continue;
        }
        //*****************************IN KEYWORD*************************************
        if(i + 2 < data_len && data_str[i] == 'i' && data_str[i + 1] == 'n' && (data_str[i + 2] == ' ')){
            struct token temp;
            temp.token_type = IN;
            temp.lexeme = "in";
            tokens[j] = temp;
j++;
            i += 2;
            continue;
        }
        //*****************************IF KEYWORD*************************************
        if(i + 2 < data_len && data_str[i] == 'i' && data_str[i + 1] == 'f' && (data_str[i + 2] == ' ' || data_str[i + 2] == '(')){
            struct token temp;
            temp.token_type = IF;
            temp.lexeme = "if";
            tokens[j] = temp;
j++;
            i += 2;
            continue;
        }
        //*****************************ELSE KEYWORD*************************************
        if(i + 4 < data_len && data_str[i] == 'e' && data_str[i + 1] == 'l' && data_str[i + 2] == 's' && data_str[i + 3] == 'e' && (data_str[i + 4] == ' ' || data_str[i + 4] == ':') ){
            struct token temp;
            temp.token_type = ELSE;
            temp.lexeme = "else";
            tokens[j] = temp;
j++;
            i += 4;
            continue;
        }
        //*****************************ELIF KEYWORD*************************************
        if(i + 4 < data_len && data_str[i] == 'e' && data_str[i + 1] == 'l' && data_str[i + 2] == 'i' && data_str[i + 3] == 'f' && (data_str[i + 4] == ' ' || data_str[i + 4] == '(') ){
            struct token temp;
            temp.token_type = ELIF;
            temp.lexeme = "elif";
            tokens[j] = temp;
j++;
            i += 4;
            continue;
        }
        //*****************************RANGE KEYWORD*************************************
        if(i + 5 < data_len && data_str[i] == 'r' && data_str[i + 1] == 'a' && data_str[i + 2] == 'n' && data_str[i + 3] == 'g' && data_str[i + 4] == 'e' && data_str[i + 5] == '('){
            struct token temp;
            temp.token_type = RANGE;
            temp.lexeme = "range";
            tokens[j] = temp;
j++;
            i += 5;
            continue;
        }
        //*****************************INPUT KEYWORD*************************************
        if(i+5<data_len&&data_str[i]=='i'&&data_str[i+1]=='n'&&data_str[i+2]=='p'&&data_str[i+3]=='u'&&data_str[i+4]=='t' && data_str[i + 5] == '('){
            struct token temp;
            temp.lexeme="input";
            temp.token_type=INPUT;
            tokens[j] = temp;
j++;
            i=i+5;
            continue;
        }
        //*****************************LPAR KEYWORD*************************************
        if(data_str[i]=='('){
            struct token temp;
            temp.lexeme="(";
            temp.token_type=LPAR;
            tokens[j] = temp;
j++;
            i=i+1;
            continue;
        }
        //*****************************RPAR KEYWORD*************************************
        if(data_str[i]==')'){
            struct token temp;
            temp.lexeme=")";
            temp.token_type=RPAR;
            tokens[j] = temp;
j++;
            i=i+1;
            continue;
        }
        //*****************************PRINT KEYWORD*************************************
        if(i+4<data_len&&data_str[i]=='p'&&data_str[i+1]=='r'&&data_str[i+2]=='i'&&data_str[i+3]=='n'&&data_str[i+4]=='t'){
            struct token temp;
            temp.lexeme="print";
            temp.token_type=PRINT;
            tokens[j] = temp;
j++;
            i=i+5;
            continue;
        }
        //*****************************AND KEYWORD*************************************
        if(i+3<data_len&&data_str[i]=='a'&&data_str[i+1]=='n'&&data_str[i+2]=='d'&&data_str[i+3]==' '){
            struct token temp;
            temp.lexeme="and";
            temp.token_type=AND;
            tokens[j] = temp;
j++;
            i=i+4;
            continue;
        }
        //*****************************NOT KEYWORD*************************************
        if(i+3<data_len&&data_str[i]=='n'&&data_str[i+1]=='o'&&data_str[i+2]=='t'&&data_str[i+3]==' '){
            struct token temp;
            temp.lexeme="not";
            temp.token_type=NOT;
            tokens[j] = temp;
j++;
            i=i+4;
            continue;
        }
        //*****************************OR KEYWORD*************************************
        if(i+2<data_len&&data_str[i]=='o'&&data_str[i+1]=='r'&&data_str[i+2]==' '){
            struct token temp;
            temp.lexeme="or";
            temp.token_type=OR;
            tokens[j] = temp;
j++;
            i=i+4;
            continue;
        }
        //****************************ASSIGN **************************************************
        if(i + 2 < data_len && data_str[i] == '+' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type =ASSIGN;
            temp.lexeme = "+=";
            tokens[j] = temp;
j++;
            i += 2;
            continue;
        }
        if(i + 2 < data_len && data_str[i] == '-' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = ASSIGN;
            temp.lexeme = "-=";
            tokens[j] = temp;
j++;
            i += 2;
            continue;
        }
        if(i + 2 < data_len && data_str[i] == '*' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = ASSIGN;
            temp.lexeme = "*=";
            tokens[j] = temp;
j++;
            i += 2;
            continue;
        }
        if(i + 2 < data_len && data_str[i] == '/' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = ASSIGN;
            temp.lexeme = "/=";
            tokens[j] = temp;
j++;
            i += 2;
            continue;
        }
        //*****************************ARITHMETIC OPERATORS*************************************
        char arithmetics[7] = {'+', '-', '/', '*', ':', '.', ','};
        int arithmetic_token_no[7] = {ADD, SUB, DIV, MUL, COLON, DOT, COMMA};
        int is_arithmetic = 0;
        for(int k = 0 ;k < 7; k++){
            if(arithmetics[k] == data_str[i]){
                is_arithmetic = 1;
                char* new_str = "";
                new_str = append_char_to_str(new_str,arithmetics[k]);
                struct token temp;
                temp.lexeme = new_str;
                temp.token_type = arithmetic_token_no[k];
                i++;
                tokens[j] = temp;
j++;
                break;
            }
        }
        if(is_arithmetic == 1){
            continue;
        }
        //**********************************RELOP*************************************
        if(i + 2 < data_len && data_str[i] == '<' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = LE;
            temp.lexeme = "<=";
            tokens[j] = temp;
j++;
            i += 2;
            continue;
        }

        if(i + 2 < data_len && data_str[i] == '>' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = GE;
            temp.lexeme = ">=";
            tokens[j] = temp;
j++;
            i += 2;
            continue;
        }

        if(i + 2 < data_len && data_str[i] == '=' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = EQ;
            temp.lexeme = "==";
            tokens[j] = temp;
j++;
            i += 2;
            continue;
        }

        if(i + 2 < data_len && data_str[i] == '!' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = NE;
            temp.lexeme = "!=";
            tokens[j] = temp;
j++;
            i += 2;
            continue;
        }
        

        if(i + 1 < data_len && data_str[i] == '<'   ){
            struct token temp;
            temp.token_type = LT;
            temp.lexeme = "<";
            tokens[j] = temp;
j++;
            i += 1;
            continue;
        }

        if(i + 1 < data_len && data_str[i] == '>'  ){
            struct token temp;
            temp.token_type = GT;
            temp.lexeme = ">";
            tokens[j] = temp;
j++;
            i += 1;
            continue;
        }
        if(i + 1 < data_len && data_str[i] == '='  ){
            struct token temp;
            temp.token_type = ASSIGN;
            temp.lexeme = "=";
            tokens[j] = temp;
j++;
            i += 1;
            continue;
        }
        //*******************************BOOL*****************************************
        if(i+3<data_len&& data_str[i] == 'T' && data_str[i + 1] == 'r' && data_str[i + 2] == 'u' && data_str[i + 3] == 'e'&&(i+4==data_len||data_str[i+4]==' '||data_str[i+4]=='\n')  ){
            struct token temp;
            temp.token_type = TRUE;
            temp.lexeme = "True";
            tokens[j] = temp;
j++;
            i += 4;
        }

        
        if(i+4<data_len&& data_str[i] == 'F' && data_str[i + 1] == 'a' && data_str[i + 2] == 'l' && data_str[i + 3] == 's'&&data_str[i + 4] == 'e'&&(i+5==data_len||data_str[i+5]==' '||data_str[i+5]=='\n')){
            struct token temp;
            temp.token_type = FALSE;
            temp.lexeme = "False";
            tokens[j] = temp;
j++;
            i += 5;
        }
        //********************************NUM*****************************************
        if((data_str[i]>='0'&&data_str[i]<='9')){
            char* s1 = "";int t=i;
            while(t < data_len && (data_str[t]>='0'&&data_str[t]<='9')){
                s1 = append_char_to_str(s1,data_str[t]);
                t++;
            }
            struct token temp;
            temp.lexeme=s1;
            temp.token_type=NUM;
            tokens[j] = temp;
j++;
            i=t;
            continue;
        }
        //*****************************ID KEYWORD*************************************
        if((data_str[i]>='a'&&data_str[i]<='z')||(data_str[i]>='A'&&data_str[i]<='Z')||data_str[i]=='_'){
            char* s1 = "";int t=i;
            while(t<data_len&&((data_str[t]>='a'&&data_str[t]<='z')||(data_str[t]>='A'&&data_str[t]<='Z')||data_str[t]=='_'||(data_str[t]>='0'&&data_str[t]<='9'))){
                s1 = append_char_to_str(s1,data_str[t]);
                // printf("Error detected");
                t++;
            }
            struct token temp;
            temp.lexeme=s1;
            temp.token_type=IDEN;
            tokens[j] = temp;
            j++;
            i=t;
            continue;
        }

    }
}
int is_cond(struct token* tokens,int i, int exp_tok){
    int state_no=0;int par_cnt=0;
    while(i<token_cnt&&(tokens[i].token_type!=exp_tok||exp_tok==EOFT)){
        if(par_cnt<0){
            is_paranthesis_error = 1;
            return 0;
        }
        if(state_no==0){
            if(tokens[i].token_type==NOT){
                i++;
                continue;
            }
            if(tokens[i].token_type==LPAR){
                par_cnt++;
                i++;
                continue;
            }
            if(tokens[i].token_type==IDEN||tokens[i].token_type==NUM){
                state_no=1;
                i++;
                continue;
            }
        }
        if(state_no==1){
            int acceptable_tokens[12] = {LE, EQ, GE, NE, LT, GT, ADD, SUB, MUL, DIV, AND, OR};
            if(tokens[i].token_type==RPAR){
                i++;
                par_cnt--;
                continue;
            }
            int is_accepted=0;
            for(int j=0;j<12;j++){
                if(tokens[i].token_type==acceptable_tokens[j]){
                    i++;
                    state_no=0;
                    is_accepted=1;
                    break;
                }
            }
            if(is_accepted==1){
                continue;
            }
        }
        return 0;
    }
    if(i==token_cnt&&exp_tok!=EOFT){
        return 0;

    }
    if(par_cnt!=0){
        is_paranthesis_error = 1;
        return 0;
    }
    return 1;
}
int is_typo(struct token* tokens){
    int line_no=1;
    for(int i = 0; i < token_cnt;){
        // printf("%s\n",tokens[i].lexeme);
        if(tokens[i].token_type==INDEN){
            // printf("ddbg");
            i++;
            line_no++;
            continue;
        }
        // ********************************** for ID in ID: *********************************************
        if(i+4<token_cnt&&tokens[i].token_type==FOR&&tokens[i+1].token_type==IDEN&&tokens[i+2].token_type==IN&&tokens[i+3].token_type==IDEN&&tokens[i+4].token_type==COLON){
            i+=5;
            continue;
        }
        // ********************************** for ID in range(NUM): *************************************
        if(i+4<token_cnt&&tokens[i].token_type==FOR&&tokens[i+1].token_type==IDEN&&tokens[i+2].token_type==IN&&tokens[i+3].token_type==RANGE&&tokens[i+4].token_type==LPAR){
            int t=i+5;
            while(t<token_cnt&&tokens[t].token_type!=RPAR){
                t++;
            }
            if(t+1<token_cnt&&tokens[t+1].token_type==COLON){
                t+=2;
                i=t;
                continue;
            }
           
        }
        // ********************************** while {is_cond}:*******************************************
        if(i+1<token_cnt&&tokens[i].token_type==WHILE&&is_cond(tokens,i+1,COLON)){
            while(tokens[i].token_type!=COLON){
                i++;
            }
            i++;
            continue;
        }
        // ********************************** if {is_cond}:**********************************************
        if(i+1<token_cnt&&tokens[i].token_type==IF&&is_cond(tokens,i+1,COLON)){
            while(tokens[i].token_type!=COLON){
                i++;
            }
            i++;
            continue;
        }
        // ********************************** elif {is_cond}:********************************************
        if(i+1<token_cnt&&tokens[i].token_type==ELIF&&is_cond(tokens,i+1,COLON)){
            while(tokens[i].token_type!=COLON){
                i++;
            }
            i++;
            continue;
        }
        // ********************************** else {is_cond}:********************************************
        if(i+1<token_cnt&&tokens[i].token_type==ELSE && is_cond(tokens,i+1,COLON)){
            while(tokens[i].token_type!=COLON){
                i++;
            }
            i++;
            continue;
        }
        // ********************************** print()****************************************************
        if(i+3<token_cnt&&tokens[i].token_type==PRINT&&tokens[i+1].token_type==LPAR&&tokens[i+2].token_type==STR&&tokens[i+3].token_type==RPAR){
            i+=4;
            continue;
        }
        if(i+2<token_cnt&&tokens[i].token_type==PRINT&&tokens[i+1].token_type==LPAR&&is_cond(tokens,i+2,RPAR)){
            i+=2;
            while(i<token_cnt&&tokens[i].token_type!=RPAR){
                i++;
            }
            i++;
            continue;
        }
        // ********************************** id=input(str)****************************************************
        if(i+3<token_cnt&&tokens[i].token_type==IDEN&&tokens[i+1].token_type==ASSIGN&&tokens[i+2].token_type==INPUT&&tokens[i+3].token_type==LPAR&&tokens[i+4].token_type==STR&&tokens[i+5].token_type==RPAR){
            i+=4;
            continue;
        }
        // ********************************** ID = {is_expr}*********************************************
        if(i+2<token_cnt&&tokens[i].token_type==IDEN&&tokens[i+1].token_type==ASSIGN&&(is_cond(tokens,i+2,INDEN)||is_cond(tokens,i+2,EOFT))){

            i=i+2;
            while(i<token_cnt&&tokens[i].token_type!=INDEN){
                i++;
            }
            continue;
        }
        // ********************************** ID = STR***************************************************
        if(i+3<token_cnt&&tokens[i].token_type==IDEN&&tokens[i+1].token_type==ASSIGN&&tokens[i].token_type==STR){
            i=i+3;
            continue;
        }
        return line_no;
        
    }
    return 0;
}
int main(){
    char* data_str = file_to_str();
    int error_info = is_indentation_error(data_str);
    if(error_info != 0){
        printf("IndentationError in line %d", error_info);
        return 0;
        // print_error
    }
    int cnt = get_tokens_cnt(data_str);
    if(is_lexical_error==1){
        printf("Unidentified symbol in line :%d",cnt);
        return 0;
    }
    token_cnt=cnt;
    // printf("No of tokens = %d\n", cnt);
    struct token tokens[cnt];
    get_tokens(data_str, tokens);
    // for(int i = 0 ; i < cnt; i++){
    //     printf("%s\n", tokens[i].lexeme);
    // }
    int error=is_typo(tokens);
    // error_info = is_typo(data_str);
    if(error!= 0){
        if(is_paranthesis_error){
            printf("Unbalanced paranthesis at line %d", error);
            return 0;
        }
        printf("Syntax error at line %d", error);
        return 0;
    }
    printf("NO Errors :)");
}
