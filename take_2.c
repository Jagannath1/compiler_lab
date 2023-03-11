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

char* INPUT_FILE = "input.txt";
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
struct token* get_tokens(char* data_str){
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
                strncat(str, &data_str[i], 1);
                i++;
            }
            struct token temp;
            temp.lexeme = str;
            temp.token_type = STR;
            continue;
        }
        if(data_str[i] == '\"'){
            char* str = "";
            i++;
            while (i < data_len && (data_str[i - 1] == '\\' || data_str[i] != '\"')){
                strncat(str, &data_str[i], 1);
                i++;
            }
            struct token temp;
            temp.lexeme = str;
            temp.token_type = STR;
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
            tokens.push_back(temp);
            continue;
        }
        //*****************************IF KEYWORD*************************************
        if(i + 2 < data_len && data_str[i] == 'i' && data_str[i + 1] == 'f' && (data_str[i + 2] == ' ' || data_str[i + 2] == '(')){
            struct token temp;
            temp.token_type = IF;
            tokens.push_back(temp);
            i += 2;
        }
        //*****************************ELSE KEYWORD*************************************
        if(i + 4 < data_len && data_str[i] == 'e' && data_str[i + 1] == 'l' && data_str[i + 2] == 's' && data_str[i + 3] == 'e' && (data_str[i + 4] == ' ' || data_str[i + 4] == ':') ){
            struct token temp;
            temp.token_type = ELSE;
            tokens.push_back(temp);
            i += 4;
        }
        //*****************************ELIF KEYWORD*************************************
        if(i + 4 < data_len && data_str[i] == 'e' && data_str[i + 1] == 'l' && data_str[i + 2] == 'i' && data_str[i + 3] == 'f' && (data_str[i + 4] == ' ' || data_str[i + 4] == '(') ){
            struct token temp;
            temp.token_type = ELIF;
            tokens.push_back(temp);
            i += 4;
        }
        //*****************************RANGE KEYWORD*************************************
        if(i + 5 < data_len && data_str[i] == 'r' && data_str[i + 1] == 'a' && data_str[i + 2] == 'n' && data_str[i + 3] == 'g' && data_str[i + 4] == 'e' && data_str[i + 5] == '('){
            struct token temp;
            temp.token_type = RANGE;
            tokens.push_back(temp);
            i += 5;
        }
        //*****************************INPUT KEYWORD*************************************
        if(i+5<data_len&&data_str[i]=='i'&&data_str[i+1]=='n'&&data_str[i+2]=='p'&&data_str[i+3]=='u'&&data_str[i+4]=='t' && data_str[i + 5] == '('){
            struct token temp;
            temp.lexeme="input";
            temp.token_type=INPUT;
            push_back(tokens,temp);
            i=i+5;
            continue;
        }
        //*****************************LPAR KEYWORD*************************************
        if(data_str[i]=='('){
            struct token temp;
            temp.lexeme="(";
            temp.token_type=LPAR;
            push_back(tokens,temp);
            i=i+1;
            continue;
        }
        //*****************************RPAR KEYWORD*************************************
        if(data_str[i]==')'){
            struct token temp;
            temp.lexeme=")";
            temp.token_type=RPAR;
            push_back(tokens,temp);
            i=i+1;
            continue;
        }
        //*****************************PRINT KEYWORD*************************************
        if(i+4<data_len&&data_str[i]=='p'&&data_str[i+1]=='r'&&data_str[i+2]=='i'&&data_str[i+3]=='n'&&data_str[i+4]=='t'){
            struct token temp;
            temp.lexeme="print";
            temp.token_type=PRINT;
            push_back(tokens,temp);
            i=i+5;
            continue;
        }
        //*****************************AND KEYWORD*************************************
        if(i+3<data_len&&data_str[i]=='a'&&data_str[i+1]=='n'&&data_str[i+2]=='d'&&data_str[i+3]==' '){
            struct token temp;
            temp.lexeme="and";
            temp.token_type=AND;
            push_back(tokens,temp);
            i=i+4;
            continue;
        }
        //*****************************NOT KEYWORD*************************************
        if(i+3<data_len&&data_str[i]=='n'&&data_str[i+1]=='o'&&data_str[i+2]=='t'&&data_str[i+3]==' '){
            struct token temp;
            temp.lexeme="not";
            temp.token_type=NOT;
            push_back(tokens,temp);
            i=i+4;
            continue;
        }
        //*****************************OR KEYWORD*************************************
        if(i+2<data_len&&data_str[i]=='o'&&data_str[i+1]=='r'&&data_str[i+2]==' '){
            struct token temp;
            temp.lexeme="or";
            temp.token_type=OR;
            push_back(tokens,temp);
            i=i+4;
            continue;
        }
        //*****************************ARITHMETIC OPERATORS*************************************
        char arithmetics[7] = {'+', '-', '/', '*', ':', '.', ','};
        int arithmetic_token_no = {ADD, SUB, DIV, MUL, COLON, DOT, COMMA};
        int is_arithmetic = 0;
        for(int j = 0 ;j < 7; j++){
            if(arithmetics[j] == data_str[i]){
                is_arithmetic = 1;
                struct token temp;
                temp.lexeme = arithmetics[j];
                temp.token_type = arithmetic_token_no[j];
                i++;
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
            tokens.push_back(temp);
            i += 2;
        }

        if(i + 2 < data_len && data_str[i] == '>' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = GE;
            tokens.push_back(temp);
            i += 2;
        }

        if(i + 2 < data_len && data_str[i] == '=' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = EQ;
            tokens.push_back(temp);
            i += 2;
        }

        if(i + 2 < data_len && data_str[i] == '!' && data_str[i + 1] == '='  ){
            struct token temp;
            temp.token_type = NE;
            tokens.push_back(temp);
            i += 2;
        }

        if(i + 1 < data_len && data_str[i] == '<'   ){
            struct token temp;
            temp.token_type = LT;
            tokens.push_back(temp);
            i += 1;
        }

        if(i + 1 < data_len && data_str[i] == '>'  ){
            struct token temp;
            temp.token_type = GT;
            tokens.push_back(temp);
            i += 1;
        }
        //*******************************BOOL*****************************************
        if(i+3<data_len&& data_str[i] == 'T' && data_str[i + 1] == 'r' && data_str[i + 2] == 'u' && data_str[i + 3] == 'e'&&(i+4==data_len||data_str[i+4]=' '||data_str[i+4]='\n')  ){
            struct token temp;
            temp.token_type = TRUE;
            tokens.push_back(temp);
            i += 4;
        }

        
        if(i+4<data_len&& data_str[i] == 'F' && data_str[i + 1] == 'a' && data_str[i + 2] == 'l' && data_str[i + 3] == 's'&&data_str[i + 4] == 'e'&&(i+5==data_len||data_str[i+5]=' '||data_str[i+5]='\n')){
            struct token temp;
            temp.token_type = FALSE;
            tokens.push_back(temp);
            i += 5;
        }
        //********************************NUM*****************************************
        if((data_str[i]>='0'&&data_str[i]<='9')){
            char* s1;int t=i;
            while((data_str[t]>='0'&&data_str[t]<='9')){
                strncat(s1, &data_str[t], 1);
                t++;
            }
            struct token temp;
            temp.lexeme=s1;
            temp.token_type=NUM;
            push_back(tokens,temp);
            i=t;
            continue;
        }
        //*****************************ID KEYWORD*************************************
        if((data_str[i]>='a'&&data_str[i]<='z')||(data_str[i]>='A'&&data_str[i]<='Z')||data_str[i]=='_'){
            char* s1;
            while(((data_str[i]>='a'&&data_str[i]<='z')||(data_str[i]>='A'&&data_str[i]<='Z')||data_str[i]=='_')&&i<data_len){
                strncat(s1, &data_str[i], 1);
                i++;
            }
            struct token temp;
            temp.lexeme=s1;
            temp.token_type=IDEN;
            push_back(tokens,temp);
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
    get_tokens(data_str);
    // error_info = is_typo(data_str);
    // if(error_info != 0){
    //     printf("Typo at line %d", error_info);
    // }
}
