/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Miyuki Ishige, 1379202
   Dated: August 30th, 2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_ONE  1      /* integer 1 */
#define INT_TEN  10	/* integer 10 */

/* Placeholder typedef for skeleton code
*/
typedef int longint_t[INTSIZE+1];
#define LONG_ZERO 0
#define LONG_ONE  1

/* Debugging codes */
#define DEBUG 1
    #if DEBUG
    #define DUMP_INT(x) printf("line %d: %s = %d\n", __LINE__, #x, x)
    #else
    #define DUMP_INT(x)
    #endif


/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg, 
    longint_t second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t var);
void do_assign(longint_t var1, longint_t var2);
void do_plus(longint_t var1, longint_t var2);
void do_mult(longint_t var1, longint_t var2);
void do_powr(longint_t var1, longint_t var2);
void zero_vars(longint_t vars[], int num_rows);
void parse_num(longint_t second_value, char *rhsarg);
void reset_arr(longint_t var);
void check_overflow(int len_longint);


/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {
    char line[LINELEN+1] = {0};
    longint_t vars[NVARS];

    zero_vars(vars, NVARS);
    print_prompt();
    while (read_line(line, LINELEN)) {
	if (strlen(line) > 0) {
	    /* non empty line, so process it */
	    process_line(vars, line);
	}
	    print_prompt();
    }

    print_tadaa();
    return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
   can be confirmed that the input is coming from a terminal.
   Plus, output might be going to a file, that's why the prompt,
   if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
    if (isatty(fileno(stdin))) {
	fprintf(stderr, "> ");
	fflush(stderr);
    }
}

void
print_tadaa() {
    /* all done, so pack up bat and ball and head home,
        getting the exact final lines right is a bit tedious,
        because input might be coming from a file and output
        might be going to a file */
    if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
	printf("\n");
    }
    printf("ta daa!!!\n");
    if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
	fprintf(stderr, "\n");
    }
}

void
print_error(char *message) {
    /* need to write an error message to the right place(s)
    */
    if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
	fprintf(stderr, "%s\n", message);
	fflush(stderr);
    }
    if (!isatty(fileno(stdout))) {
	printf("%s\n", message);
    }
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available,
   all whitespace characters are removed on the way through
*/
int
read_line(char *line, int maxlen) {
    int i=0, c;
    while (((c=getchar())!=EOF) && (c!='\n')) {
	if (i<maxlen && !isspace(c)) {
	    line[i++] = c;
	}
    }
    line[i] = '\0';
    /* then, if the input is coming from a file or the output
        is going to a file, it is helpful to echo the input line
	and record what the command was */
    if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
	printf("%s%s\n", PROMPT, line);
    }
	
    return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
*/
void
process_line(longint_t vars[], char *line) {
    int varnum, optype, status;
    longint_t second_value;

    /* determine the LHS variable, it
       must be first character in compacted line
    */
    varnum = to_varnum(line[0]);
    if (varnum==ERROR) {
	print_error("invalid LHS variable");
	return;
    }

    /* more testing for validity 
    */
    if (strlen(line)<2) {
	print_error("no operator supplied");
	return;
    }

    /* determine the operation to be performed, it
	must be second character of compacted line
    */
    optype = line[1];
    if (strchr(ALLOPS, optype) == NULL) {
	print_error("unknown operator");
	return;
    }

    /* determine the RHS argument (if one is required),
	it must start in the third character of compacted line
    */
    if (optype != PRINT) {
	if (strlen(line)<3) {
	    print_error("no RHS supplied");
	    return;
	}
		
        status = get_second_value(vars, line+2, second_value);

        if (status==ERROR) {
	    print_error("RHS argument is invalid");
	    return;
        }
    }

    /* finally, do the actual operation
    */
    if (optype == PRINT) {
	do_print(varnum, vars[varnum]);
    } else if (optype == ASSIGN) {
	do_assign(vars[varnum], second_value);
    } else if (optype == PLUS) {
	do_plus(vars[varnum], second_value);
    /* you will need to add further operators here */
    } else if (optype == MULT){
        do_mult(vars[varnum], second_value);
    } else if (optype == POWR){
        do_powr(vars[varnum], second_value);
    } else {
	print_error("operation not available yet");
	return;
    }
    return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
    int varnum;
    varnum = ident - CH_A;
    if (0<=varnum && varnum<NVARS) {
	return varnum;
    } else {
	return ERROR;
    }
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(longint_t vars[], char *rhsarg, longint_t second_value) {
    char *p;
    int varnum2;

    if (isdigit(*rhsarg)) {
	/* first character is a digit, so RHS is a number
	    now check the rest of RHS for validity */
	for (p=rhsarg+1; *p; p++) {
	    if (!isdigit(*p)) {
	        /* nope, found an illegal character */
		return ERROR;
	    }
	}
	/* nothing wrong, ok to convert */
	parse_num(second_value, rhsarg);	
	return !ERROR;
    } else {
	/* argument is not a number, so should be a variable */
	varnum2 = to_varnum(*rhsarg);
	if (varnum2==ERROR || strlen(rhsarg)!=1) {
	    /* nope, not a variable either */
	    return ERROR;
	}

	/* and finally, get that variable's value */
	do_assign(second_value, vars[varnum2]);
	return !ERROR;
    }

    return ERROR;
}

/****************************************************************/

/* Helper function, sets the all values in 2D vars array to INT_ZERO */
void
zero_vars(longint_t vars[], int num_rows) {
    int i;
    int zero_arr[INTSIZE+1] = {LONG_ZERO};

    for (i=0; i<num_rows; i++) {
	do_assign(vars[i], zero_arr);
    }

    return;
}

/****************************************************************/

/* Helper function, create an longint_t format number out of a string, 
   remove any leading zeros */
void
parse_num(longint_t second_value, char *rhsarg) {
    int rhsarg_len = strlen(rhsarg);

    /* Return error and exit if rhsarg is longer than INTSIZE */
    check_overflow(rhsarg_len);

    /* rhsarg is not longer than INTSIZE */
    int i, j, k;
    int rhsarg_int, num_pos=1, nonzero_pos=-1;
    int rhsarg_arr[rhsarg_len];

    /* Create an array of digits in rhsarg */
    for (i=0; i<rhsarg_len; i++){
	char num[] = {rhsarg[i],0};
	rhsarg_int = atoi(num);
	rhsarg_arr[i] = rhsarg_int; 
    }

    /* Code written in lines 381-393 is referred to
	https://www.geeksforgeeks.org/remove-leading-zeros-from-an-array/ 
	Their approach to find the first non-zero digit in an array is used */

    /* Find the position of first non-zero integer in rhsarg_arr */
    for (j=0; j<rhsarg_len; j++){
	if (rhsarg_arr[j] != INT_ZERO){
	    nonzero_pos = j;
	    break;
	}
    }

    /* If all digits are zero in the rhsarg_arr */
    if (nonzero_pos == -1){
	second_value[0] = 1;
	second_value[1] = INT_ZERO;
	return;
    }    

    /* Copy the digits in rhsarg_arr to second_value in reversed order */
    for (k=rhsarg_len-1; k>=nonzero_pos; k--){
	second_value[num_pos] = rhsarg_arr[k];
	num_pos += 1;
    }   

    /* Put the buddy variable of second_value array as 
	length of rhsarg without leading zeros,
	-1 as the num_pos is 1 larger than the actual number of digits */
    second_value[0] = num_pos-1;
    
    return;
}


/****************************************************************/

/* Print out a longint value with a comma in every 3 digits */
void
do_print(int varnum, longint_t var) {

    printf("register %c: ", varnum+CH_A);

    int longint_len = var[0], i;

    if (longint_len == 0){
	printf("%d", longint_len);
    } else {
	for (i=longint_len; i>0; i--){
            printf("%d", var[i]);
	    if (i % PUT_COMMAS == 1 && i != 1){
		printf("%c", CH_COM);
	    }
	}
    }

    printf("\n");

    return;
}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2 */
void
do_assign(longint_t var1, longint_t var2) {
    int i;

    /* If the function is called from zero_vars,
	initialize all the digits as zero */
    if (var2[0] == 0){
	reset_arr(var1);

    /* Else assign integers, preserve the digits 
	outside of the range var2[0] in var1 */
    } else {
	/* Reset all the values to INT_ZERO in var1 */
	reset_arr(var1);

	/* Assign the values in var2 to var1 */
	for (i=0; i<var2[0]+1; i++){
	    var1[i] = var2[i];
	}
    }

    return;
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2 */
void
do_plus(longint_t var1, longint_t var2) {
    int i, j;
    int digit_sum, remainder, add_size=0;

    /* Addition when var1 has larger or equal number of digits than var2 */ 
    if (var1[0] >= var2[0]){
        for (i=1; i<var2[0]+1; i++){
	    digit_sum = var1[i] + var2[i];

	    if (digit_sum >= INT_TEN){
		remainder = digit_sum % INT_TEN;
		var1[i] = remainder;
		var1[i+1] += INT_ONE;

		/* If the number of digits in var1 and var2 are equal, 
		   and sum of last digits is greater than INT_TEN, 
		   number of digits in var1 increases by 1 */
		if (var1[0] == var2[0] && i == var2[0]){
		    add_size += 1;
		}

	    } else {
                var1[i] = digit_sum;
	    }
	}

	/* Find digit >= 10 in new var1 array 
	    and perform appropriate addition */ 
	for (j=1; j<var1[0]+1; j++){
            if (var1[j] >= INT_TEN){ 
		remainder = var1[j] % INT_TEN;
		var1[j] = remainder; 
		var1[j+1] += INT_ONE; 

		/* If the last digit in var1 is greater than INT_TEN, 
		   number of digits in var1 increases by 1 */
		if (j == var1[0]){
		    add_size += 1;
		}
	    } 
	}		

	/* Update the buddy variable of var1 */ 
	var1[0] += add_size;

    /* Addition when var1 has less number of digits than var2 */ 
    } else if (var1[0] < var2[0]){
	for (i=1; i<var2[0]+1; i++){
	    digit_sum = var1[i] + var2[i];
	    remainder = digit_sum % INT_TEN;

	    if (digit_sum >= INT_TEN && i > var1[0]){
	        var1[i] = remainder;
	        var1[i+1] += INT_ONE;
	        add_size += 1;

	        /* Number of digits in var1 increases by 1,
		   if the last digit of var2 is 9 */
	        if (i == var2[0]){
		    add_size += 1;
	        }

	    } else if (i > var1[0]) {
		var1[i] = digit_sum;
		add_size += 1;
	    } else if (digit_sum >= INT_TEN){
		var1[i] = remainder;
		var1[i+1] += INT_ONE;
	    } else {
		var1[i] = digit_sum;
	    }
	}

	/* Update the buddy variable of var1 */ 
	var1[0] += add_size;
    }

    /* If the sum is greater than INTSIZE */
    check_overflow(var1[0]);

    return;
}

/****************************************************************/

/* Update the indicated variable var1 by doing an multiplication,
   using var2 to compute var1 = var1 * var2 */
void 
do_mult(longint_t var1, longint_t var2){
    int i, j, k;
    int len_multiplier, len_multiplicand, product, add_size, increment, shift; 
    int *multiplicand, *multiplier;

    /* Create an array to store partial sums arise in multiplication */
    longint_t partial_sum[INTSIZE+1];
    zero_vars(partial_sum, INTSIZE+1);

    /* Identify multiplier and multiplicand */
    if (var1[0] >= var2[0]){
        len_multiplier = var2[0];
        len_multiplicand = var1[0];
	multiplier = var2;		
	multiplicand = var1;	
    } else {
	len_multiplier = var1[0];
	len_multiplicand = var2[0];
	multiplier = var1;
	multiplicand = var2;
    }

    /* Perform multiplication */
    for (i=1; i<len_multiplier+1; i++){
	add_size = 0; 
	increment = 0;
	shift = i-1;

	for (j=1; j<len_multiplicand+1; j++){  
            product = multiplicand[j] * multiplier[i] + increment;
            increment = 0;

            /* Keep track of how much needs to be added to next digit */
	    int copy_product = product;
	    while (product >= INT_TEN){
                product -= INT_TEN;
                increment += INT_ONE;
	    }

            /* If multiplying the last digit causes incrementation */
	    if (copy_product >= 10 && j == len_multiplicand) {
		add_size += 1;
		partial_sum[i][j+i] = increment;
	    }

	    /* shift the position of products by (i-1) right, 
		as i increase by 1, the multiplier increases by 10 */
	    partial_sum[i][j+shift] = product;
	    add_size += 1;
	}

	/* Update the buddy variable of partial_sum[i] */ 
	if (multiplier[0] == 1 && multiplier[1] == INT_ZERO) {
	    partial_sum[i][0] = 0;
	} else {
            partial_sum[i][0] = add_size + shift;
        }
    }

    /* Add all the integers stored in partial_sum
        do_plus function returns error, 
        if the sum of the partial_sums are greater than INTSIZE */ 
    for (k=1; k<len_multiplier+1; k++){
        do_plus(partial_sum[0], partial_sum[k]);
    }

    /* Product is not greater than INTSIZE, 
	copy the result into var1 array */ 
    do_assign(var1, partial_sum[0]);

    return;
}

/****************************************************************/

/* Update the indicated variable var1 by performing power, 
using var2 to compute var 1 = var1 ^ var2 */
void
do_powr(longint_t var1, longint_t var2) {
    int i, j;
    int exponent=0, len_exponent=var2[0];

    /* Get the integer value of the exponent */
    for (i=len_exponent; i>0; i--){
	exponent = exponent * INT_TEN + var2[i];
    }

    /* Create a copy array of var1 as the base of power operation */ 
    longint_t base;
    do_assign(base, var1);

    /* If base is INT_ZERO or INT_ONE, the answer is the base itself */ 
    if (base[0] == 1 && (base[1] == INT_ZERO || base[1] == INT_ONE)){
	return;

    /* If exponent is INT_ZERO, except when the base is INT_ZERO, 
       the answer will always be INT_ONE */ 
    } else if (exponent == INT_ZERO){
	reset_arr(var1);
	var1[0] = 1;
	var1[1] = INT_ONE;
	return;
    }

    /* Multiply base by exponent times */
    for (j=0; j<exponent-1; j++){
	do_mult(var1, base);
    }

    return;
}

/****************************************************************/

/* Helper function, "reset" the 1D array passed in
   by replacing INT_ZERO into all digits,
   used in do_assign and do_powr functions */
void 
reset_arr(longint_t var) {
    int i;

    for (i=0; i<INTSIZE+1; i++){
	var[i] = INT_ZERO;
    }

    return;
}

/****************************************************************/

/* Helper function, Check if the integer is within INT_SIZE */

void 
check_overflow(int len_longint) {
    if (len_longint > INTSIZE){
        print_error("Too large to calculate!");
	exit(EXIT_FAILURE);
    }

    return; 
}

/****************************************************************/

/* algorithms are fun */