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
int token_cnt = 0;

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
    int cnt = 0;
    int data_len = strlen(data_str);
    for(int i = 0 ; i < data_len;){
        printf("%c ",data_str[i]);
        printf("%d\n",cnt);
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
            struct token temp;
            temp.lexeme = str;
            temp.token_type = STR;
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
            temp.token_type = INDEN;
            cnt++;
            continue;
        }
        //******************************FOR******************************************
        if(i + 3 < data_len && data_str[i] == 'f' && data_str[i + 1] == 'o' && data_str[i+2] == 'r' && (data_str[i + 3] == ' ')){
            struct token temp;
            temp.token_type = FOR;
            cnt++;
            i += 3;
        }
        //******************************WHILE******************************************
        if(i + 5 < data_len && data_str[i] == 'w' && data_str[i + 1] == 'h' && data_str[i+2] == 'i' && data_str[i+3] == 'l'&& data_str[i+4] == 'e'&& (data_str[i + 5] == ' '||data_str[i + 5] == '(')){
            struct token temp;
            temp.token_type = WHILE;
            cnt++;
            i += 5;
        }
        //*****************************IF KEYWORD*************************************
        if(i + 2 < data_len && data_str[i] == 'i' && data_str[i + 1] == 'f' && (data_str[i + 2] == ' ' || data_str[i + 2] == '(')){
            struct token temp;
            temp.token_type = IF;
            cnt++;
            i += 2;
        }
        //*****************************ELSE KEYWORD*************************************
        if(i + 4 < data_len && data_str[i] == 'e' && data_str[i + 1] == 'l' && data_str[i + 2] == 's' && data_str[i + 3] == 'e' && (data_str[i + 4] == ' ' || data_str[i + 4] == ':') ){
            struct token temp;
            temp.token_type = ELSE;
            cnt++;
            i += 4;
        }
        //*****************************ELIF KEYWORD*************************************
        if(i + 4 < data_len && data_str[i] == 'e' && data_str[i + 1] == 'l' && data_str[i + 2] == 'i' && data_str[i + 3] == 'f' && (data_str[i + 4] == ' ' || data_str[i + 4] == '(') ){
            struct token temp;
            temp.token_type = ELIF;
            cnt++;
            i += 4;
        }
        //*****************************RANGE KEYWORD*************************************
        if(i + 5 < data_len && data_str[i] == 'r' && data_str[i + 1] == 'a' && data_str[i + 2] == 'n' && data_str[i + 3] == 'g' && data_str[i + 4] == 'e' && data_str[i + 5] == '('){
            struct token temp;
            temp.token_type = RANGE;
            cnt++;
            i += 5;
        }
        //*****************************INPUT KEYWORD*************************************
        if(i+5<data_len&&data_str[i]=='i'&&data_str[i+1]=='n'&&data_str[i+2]=='p'&&data_str[i+3]=='u'&&data_str[i+4]=='t' && data_str[i + 5] == '('){
            struct token temp;
            temp.lexeme="input";
            temp.token_type=INPUT;
            cnt++;
            i=i+5;
            continue;
        }
        //*****************************LPAR KEYWORD*************************************
        if(data_str[i]=='('){
            struct token temp;
            temp.lexeme="(";
            temp.token_type=LPAR;
            cnt++;
            i=i+1;
            continue;
        }
        //*****************************RPAR KEYWORD*************************************
        if(data_str[i]==')'){
            struct token temp;
            temp.lexeme=")";
            temp.token_type=RPAR;
            cnt++;
            i=i+1;
            continue;
        }
        //*****************************PRINT KEYWORD*************************************
        if(i+4<data_len&&data_str[i]=='p'&&data_str[i+1]=='r'&&data_str[i+2]=='i'&&data_str[i+3]=='n'&&data_str[i+4]=='t'){
            struct token temp;
            temp.lexeme="print";
            temp.token_type=PRINT;
            cnt++;
            i=i+5;
            continue;
        }
        //*****************************AND KEYWORD*************************************
        if(i+3<data_len&&data_str[i]=='a'&&data_str[i+1]=='n'&&data_str[i+2]=='d'&&data_str[i+3]==' '){
            struct token temp;
            temp.lexeme="and";
            temp.token_type=AND;
            cnt++;
            i=i+4;
            continue;
        }
        //*****************************NOT KEYWORD*************************************
        if(i+3<data_len&&data_str[i]=='n'&&data_str[i+1]=='o'&&data_str[i+2]=='t'&&data_str[i+3]==' '){
            struct token temp;
            temp.lexeme="not";
            temp.token_type=NOT;
            cnt++;
            i=i+4;
            continue;
        }
        //*****************************OR KEYWORD*************************************
        if(i+2<data_len&&data_str[i]=='o'&&data_str[i+1]=='r'&&data_str[i+2]==' '){
            struct token temp;
            temp.lexeme="or";
            temp.token_type=OR;
            cnt++;
            i=i+4;
            continue;
        }
        //*****************************ARITHMETIC OPERATORS*************************************
        char arithmetics[7] = {'+', '-', '/', '*', ':', '.', ','};
        int arithmetic_token_no[7] = {ADD, SUB, DIV, MUL, COLON, DOT, COMMA};
        int is_arithmetic = 0;
        for(int j = 0 ;j < 7; j++){
            if(arithmetics[j] == data_str[i]){
                is_arithmetic = 1;
                char* new_str = "";
                new_str = append_char_to_str(new_str,arithmetics[j]);
                struct token temp;
                temp.lexeme = new_str;
                temp.token_type = arithmetic_token_no[j];
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
            struct token temp;
            temp.token_type = LE;
            cnt++;
            i += 2;
        }

        if(i + 2 < data_len && data_str[i] == '>' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = GE;
            cnt++;
            i += 2;
        }

        if(i + 2 < data_len && data_str[i] == '=' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = EQ;
            cnt++;
            i += 2;
        }

        if(i + 2 < data_len && data_str[i] == '!' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = NE;
            cnt++;
            i += 2;
        }

        if(i + 1 < data_len && data_str[i] == '<'   ){
            struct token temp;
            temp.token_type = LT;
            cnt++;
            i += 1;
        }

        if(i + 1 < data_len && data_str[i] == '>'  ){
            struct token temp;
            temp.token_type = GT;
            cnt++;
            i += 1;
        }
        if(i + 1 < data_len && data_str[i] == '='  ){
            struct token temp;
            temp.token_type = ASSIGN;
            cnt++;
            i += 1;
        }
        //*******************************BOOL*****************************************
        if(i+3<data_len&& data_str[i] == 'T' && data_str[i + 1] == 'r' && data_str[i + 2] == 'u' && data_str[i + 3] == 'e'&&(i+4==data_len||data_str[i+4]==' '||data_str[i+4]=='\n')  ){
            struct token temp;
            temp.token_type = TRUE;
            cnt++;
            i += 4;
        }

        
        if(i+4<data_len&& data_str[i] == 'F' && data_str[i + 1] == 'a' && data_str[i + 2] == 'l' && data_str[i + 3] == 's'&&data_str[i + 4] == 'e'&&(i+5==data_len||data_str[i+5]==' '||data_str[i+5]=='\n')){
            struct token temp;
            temp.token_type = FALSE;
            cnt++;
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
            cnt++;
            i=t;
            continue;
        }
        //*****************************ID KEYWORD*************************************
        if((data_str[i]>='a'&&data_str[i]<='z')||(data_str[i]>='A'&&data_str[i]<='Z')||data_str[i]=='_'){
            char* s1;
            while(((data_str[i]>='a'&&data_str[i]<='z')||(data_str[i]>='A'&&data_str[i]<='Z')||data_str[i]=='_')&&i<data_len){
                s1 = append_char_to_str(s1,data_str[i]);
                i++;
            }
            struct token temp;
            temp.lexeme=s1;
            temp.token_type=IDEN;
            cnt++;
            continue;
        }
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
        }

        if(i + 2 < data_len && data_str[i] == '>' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = GE;
            temp.lexeme = ">=";
            tokens[j] = temp;
j++;
            i += 2;
        }

        if(i + 2 < data_len && data_str[i] == '=' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = EQ;
            temp.lexeme = "==";
            tokens[j] = temp;
j++;
            i += 2;
        }

        if(i + 2 < data_len && data_str[i] == '!' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = NE;
            temp.lexeme = "!=";
            tokens[j] = temp;
j++;
            i += 2;
        }

        if(i + 1 < data_len && data_str[i] == '<'   ){
            struct token temp;
            temp.token_type = LT;
            temp.lexeme = "<";
            tokens[j] = temp;
j++;
            i += 1;
        }

        if(i + 1 < data_len && data_str[i] == '>'  ){
            struct token temp;
            temp.token_type = GT;
            temp.lexeme = ">";
            tokens[j] = temp;
j++;
            i += 1;
        }
        if(i + 1 < data_len && data_str[i] == '='  ){
            struct token temp;
            temp.token_type = ASSIGN;
            temp.lexeme = "=";
            tokens[j] = temp;
j++;
            i += 1;
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
            while(((data_str[t]>='a'&&data_str[t]<='z')||(data_str[t]>='A'&&data_str[t]<='Z')||data_str[t]=='_'||(data_str[t]>='0'&&data_str[t]<='9'))&&t<data_len){
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
int main(){
    char* data_str = file_to_str();
    int error_info = is_indentation_error(data_str);
    if(error_info != 0){
        printf("IndentationError in line %d", error_info);
        return 0;
        // print_error
    }
    int cnt = get_tokens_cnt(data_str);
    printf("No of tokens = %d\n", cnt);
    struct token tokens[cnt];
    get_tokens(data_str, tokens);
    for(int i = 0 ; i < cnt; i++){
        printf("%s\n", tokens[i].lexeme);
    }
    // error_info = is_typo(data_str);
    // if(error_info != 0){
    //     printf("Typo at line %d", error_info);
    // }
}
