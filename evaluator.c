#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Valid Inputs
char mathOperator[] = "+-*/%";
const char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char lower[] = "abcdefghijklmnopqrstuvwxyz";

const char digit[] = "0123456789";

const char sp_char[] = " .+-*/%<> =!&|(){}[];:,\"'_";

const char* integer[] = {"digit*", "-digit*"};

const char* char_set[] = {
    upper,
    lower,
    digit,
    sp_char
};

// Valid Reserved Words and Keywords
const char *ReservedWord[] = {
    "ip",
    "subnet",
    "int",
    "float",
    "char",
    "string",
    "bool",
    "func",
    "return",
    "if",
    "else",
    "for",
    "in",
    "while",
    "do",
    "break",
    "continue",
    "contains",
    "and",
    "or",
    "not",
    "print",
    "true",
    "false",
    "null",
    "schedule",
    "every",
    "mac",
    "port",
    "ipv6",
    "throw"
};

// Creates a dynamic counter for the number of reserved words
const int ReservedwordCount = sizeof(ReservedWord) / sizeof(ReservedWord[0]);

// All the possible states to go to given an input.

/*
	NOTE: The Numbering of the states was followed based on the documentation, and the arithmetic and assignment operators' 
	machine was combined into one for simplicity and efficiency. The machines for the keywords was also omitted and was instead
	handled by comparing the input with the values in the valid reserved words for simplicity, efficiency, and readability of the
	whole codebase.
*/
enum DFAStates{
	Q1, // Start State
	
	Q2, // Charset States
	
	Q3, Q4, //Identifier States
	
	Q5, Q6, //Assignment and Arithmetic Operators States
	
	Q8, Q9, Q10, //Unary Operators States
	
	Q11, Q12, Q13, //Logical Operators States
	
	Q14, Q15, Q16, //Relational Operators States
	
	Q17, Q18, Q19, //Bitwise Operators States
	
	Q162, Q163, //Single Line Comment States
	
	Q164, Q165, Q166, Q167, //Multiple Line Comment States
	
	Q168, Q169, //String Literal States
	
	Q170, //Integer Literal States
	
	Q171, Q172, //Float Literal States
	
	Q173, Q174, //IP Literal States
	
	Q175, Q176, //Subnet Literal States
	
	QDead //Trapping or Dead State for invalid inputs
};

//A function that determines which machine to go from the start state.
enum DFAStates machineClassifier(char c, char inputString[]) {
	int inputLength = strlen(inputString);
	
	// It evaluates the input and return a state depending on that input
	if(strchr(mathOperator, c)) {
		if(inputLength == 2) {
			if(c == '-' || c == '+') {
				if(inputString[1] == '-') {
					return Q10;
				}
				if(inputString[1] == '+'){
					return Q8;
				}
			}
			
			else if (c == '/') {
				if(inputString[1] == '/') {
					return Q162;
				}
				if(inputString[1] == '*') {
					return Q164;
				}
			}
				
		}
		return Q6;
	}
	else if (c == '=')
	{
		return Q5;
	} 
	else if (c == '|') {
		if(inputLength == 2) {
			return Q11;
		}
		return Q18;
	}
	else if (c == '!') {
		if(inputLength == 2) {
			return Q16;
		}
		return Q12;
	}
	else if (c == '&') {
		if(inputLength == 2) {
			return Q13;
		}
		return Q18;
	}
	else if (c == '<' || c == '>') {
		if(inputLength == 2) {
			if(inputString[1] == '<') {
				return Q17;
			}
			else if(inputString[1] == '>') {
				return Q19;
			}
		}
		return Q14;
	}
	else if (c == '^' || c == '~') {
		return Q18;
	}
	else if (strchr(upper, c) || strchr(lower, c)) {
		return Q3;
	}
	else if (c == '\"' || c == '\'') {
		return Q168;
	} 
	else if (strchr(digit, c)) {
		return Q170;
	}
	
	// If the input is invalid, it will go to the dead state
	return QDead;
}

// A function that transitions through the machine from one state to another
enum DFAStates transition(enum DFAStates state, char c) {
	// Evaluates the input depending on which state it is currently in. It returns a state depending on the input that is based on the
	// diagram on the documentation.
	switch(state) {
		case Q3:
		case Q4:
			if(strchr(upper, c) || strchr(lower, c) || strchr(digit, c)) {
				return Q3;
			}
			else if(c == '_') {
				return Q4;
			}
			return QDead;
		
		case Q5:
			if(c == '\0') {
				return Q5;
			}
			return QDead;
			
		case Q6:
			if(c == '=') {
				return Q5;
			}
			return QDead;
			;
			
		case Q8:
			if(c == '+') {
				return Q9;
			}
			return QDead;
			
			
		case Q10:
			if(c == '-') {
				return Q9;
			}
			return QDead;
			
			
		case Q11:
			if(c == '|') {
				return Q12;
			}
			return QDead;
			
			
		case Q12:
			if(c == '\0') {
				return Q12;
			}
			return QDead;
			
			
		case Q13:
			if(c == '&') {
				return Q12;
			}
			return QDead;
			
		
		case Q14:
		case Q16:
			if(c == '=') {
				return Q15;
			}
			return QDead;
			
			
		case Q15:
			if(c == '\0') {
				return Q15;
			}
			return QDead;
			
			
		case Q17:
			if(c == '<') {
				return Q18;
			}
			return QDead;
			
			
		case Q18:
			if(c == '\0') {
				return Q18;
			}
			return QDead;
			
		
		case Q19:
			if(c == '>') {
				return Q18;
			}
			return QDead;
			
		
		case Q162:
			if(c =='/') {
				return Q163;
			}
			return QDead;
			
			
		case Q163:
			if(c == '\n') {
				return QDead;
			}
			return Q163;
			
		case Q164:
			if(c == '*') {
				return Q165;
			}
			return QDead;
			
		
		case Q165:
			if(c != '*') {
				return Q165;
			}
			else {
				return Q166;
			}
			return QDead;
			
			
		case Q166:
			if(c == '*') {
				return Q166;
			}
			else if(c == '/') {
				return Q167;
			}
			else {
				return Q165;
			}
			return QDead;
			
		case Q167:
			if(c == '\0') {
				return Q167;
			}
			return QDead;
		
		case Q168:
			if(c == '\"' || c == '\'') {
				return Q169;
			}
			return Q168;
			
		case Q169:
			if(c == '\0') {
				return Q169;
			}
			return QDead;
			
		case Q170:
			if(strchr(digit, c)) {
				return Q170;
			}
			else if(c == '.') {
				return Q171;
			}
			return QDead;
			
		case Q171:
			if(strchr(digit, c)) {
				return Q171;
			}
			else if(c == 'F' || c == 'f') {
				return Q172;
			} else if(c == '.') {
				return Q173;
			}
			return QDead;
		
		case Q172:
			if(c == '\0') {
				return Q172;
			}
			return QDead;
		
		case Q173:
			if(c == '.') {
				return Q174; 
			} else if (strchr(digit, c)) {
				return Q173;
			}
			return QDead;
		
		case Q174:
			if(c == '/') {
				return Q175;
			}
			else if (strchr(digit, c)) {
				return Q174;
			}
			return QDead;
			
		case Q175:
			if(strchr(digit, c)) {
				return Q176;
			}
			return QDead;
			
		case Q176:
			if(strchr(digit, c)) {
				return Q176;
			}
			return QDead;
			
		case QDead:
			return QDead;
			
	}
	
	return QDead;
}

// A function that evaluates whether the input is a reserved word
int isReservedWord(char inputString[]) {
	for(int i = 0; i < ReservedwordCount; i++) {
		if(strcmp(ReservedWord[i], inputString) == 0) {
			return 1;
		}
	}
	return 0;
}

// A function that takes the input and outputs the grouping based on the final state it is in.
int main(char **input, int count) {
    int resultCount = 0;
    enum DFAStates currentState = Q1; // The initialization of the start state
	
	char **result = calloc(count, sizeof(char *));
    
    for(int i = 0; i < count; i++){
    	result[i] = malloc(100 * sizeof(char));

        if (result[i] == NULL) {
        	printf("Memory allocation failed\n");
            break;
        }

        resultCount++;
        
        //Creates a grouping based on the final state
		if(isReservedWord(input[i])) {
			strcpy(result[i], "Keywords");
		}
		else {
			size_t len = strlen(input[i]);
			
			currentState = machineClassifier(input[i][0], input[i]);
			
			for (int j = 1; j < len; j++) {
			    currentState = transition(currentState, input[i][j]);
			}
			switch (currentState) {
			    case Q3:
			        strcpy(result[i], "Identifier");
			        break;
			
			    case Q5:
			        strcpy(result[i], "Assignment Operator");
			        break;
			
			    case Q6:
			        strcpy(result[i], "Arithmetic Operator");
			        break;
			
			    case Q8:
			    case Q9:
			    case Q10:
			        strcpy(result[i], "Unary Operator");
			        break;
			
			    case Q12:
			        strcpy(result[i], "Logical Operator");
			        break;
			
			    case Q14:
			    case Q15:
			        strcpy(result[i], "Relational Operator");
			        break;
			
			    case Q17:
			    case Q18:
			        strcpy(result[i], "Bitwise Operator");
			        break;
			
			    case Q163:
			    case Q167:
			        strcpy(result[i], "Comment");
			        break;
			    
			    case Q169:
			    	strcpy(result[i], "String Literal");
			    	break;
			    	
			    case Q170:
			    	strcpy(result[i], "Integer Literal");
			    	break;
			    	
			    case Q171:
			    case Q172:
			    	strcpy(result[i], "Float Literal");
			    	break;
			    
			    case Q174:
			    	strcpy(result[i], "IP Literal");
			    	break;
			    	
			    case Q176:
			    	strcpy(result[i], "Subnet Literal");
			    	break;
			
			    default:
			        strcpy(result[i], "Unknown Token");
			        break;
			}
		}
	}
	
	//Outputs the result
	for(int i = 0; i < count; i++) {
		printf("\nLexeme: %s => Result: %s", input[i], result[i]);
	}
	
	//Free the memory
	free(input);
	free(result);
	
	return 0;
}
	