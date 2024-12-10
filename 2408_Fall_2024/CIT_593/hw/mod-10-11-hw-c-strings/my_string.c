/**************************************************************
 * file name   : my_string.c                                  *
 * author      : tjf & you                                    *
 * description : this C-file defines the functions previously *
 *               declared in the my_string.h header file      *
 *                                                            *
 *                                                            *
 **************************************************************
 *
 */
#include <stdio.h>		// Added for NULL.
#include "my_string.h"

// strlen functions takes in a pointer to a string and return its length
//
size_t2 my_strlen  (const char *str) {

	size_t2 len = 0 ;
	while (str[len] != '\0') {  // count up to the first NULL
		len++ ; 
	}
	return (len) ;

}

size_t2 my_strlen2 (const char *str) {

	const char* s;
	for (s = str; *s; ++s) ;
	return (s - str);

}


/* CIT 593 students: TODO: implement the remaining string functions
   that were declared in my_string.h */

// strcpy functions takes in a destination pointer and a string pointer, and copy str into dest.
//
char* my_strcpy  (char *dest, const char *src) {
	int i=0;					// Put an index variable i.
	while (src[i] != '\0'){		// Loop until it reaches the end.
		dest[i] = src[i];		// Copy the value at src+i into dest[i].
		i++;					// Increment i.
	}
	dest[i] = '\0';				// Add '\0' at the end of the dest array.
	return dest;
}

char* my_strcpy2 (char *dest, const char *src) {
	char* result = dest;		// Copy the address of dest into result.
	while (*src){				// Loop until *src is '\0'.
		*dest = *src;			// Copy the value *src into dest.
		dest++;					// Increment dest.
		src++;					// Increment src.
	}	
	*dest = '\0';				// Add '\0' at the end of the dest array.
	return result;
}



// strchr functions takes a string pointer and a char, and returns the pointer of the first occurence of char in str.
//
char* my_strchr  (const char *str, int c) {	
	for (int i=0; str[i]; i++){			// Increment i while str[i] != '\0'
		if (str[i] == c) {
			return (char*) (str + i);	// If found, return char pointer casted (str+i)
		}
	}
	return NULL;						// If not found during the loop, return NULL.
}

char* my_strchr2 (const char *str, int c) {
	while (*str){						// Loop while str[i] != '\0'
		if (*str == c){	
			return (char*) str;			// If found, return char pointer casted str
		}
		str++;							// Increment the pointer str.
	}
	return NULL;     					// If not found during the loop, return NULL.
}


// strcat functions takes destination and src char pointers, concatenate src at the end of dest, and return dest. */
//
char* my_strcat  (char *dest, const char *src) {
	int i=0;						// Index i=0.
	while (dest[i]){				// While dest[i] != '\0'
		i++;						// Increment i.				
	}	
	for (int j=0; src[j]; j++){		// while src[j] != '\0'
		dest[i] = src[j];			// Copy dest[i] = src[j]
		i++;						// Increment i.
	}
	dest[i] = '\0';					// Add '\0' at the end of dest.
	return dest;
}

char* my_strcat2 (char *dest, const char *src) {
	char* temp = dest;				// Copy the pointer of dest to temp.
	while (*temp){					// while *temp != '\0'
		temp++;						// Increment temp.
	}
	while (*src){					// while *src != '\0'
		*temp = *src;				// Copy *temp = *src.
		temp++;						// Increment temp.
		src++;						// Increment src.
	}
	*temp = '\0';					// Add '\0' at the end of temp.
	return dest;
}


/* strcmp functions takes two string, compare them, and return 0                  if equal, 
															   (negative integer) if the first string is less than second string lexicographically
															   (positive integer) if the first string is greater than second string lexicographically */
int strcmp  (const char *str1, const char *str2) {
	int i=0;												// Initialize index i = 0.
	while (str1[i] && str2[i] && (str1[i] == str2[i])){		// While str1[i] and str2[i] are not end and the char are the same
		i++;												// Increment i.
	}
	return (int) (str1[i] - str2[i]);						// return the int casted value of str1[i] - str2[i]
}

int strcmp2 (const char *str1, const char *str2) {			
	while(*str1 && *str2 && (*str1 == *str2)){			// While str1 and str2 are not end and the char value are the same
		str1++;											// Increment str1.
		str2++;											// Increment str2.
	}
	return (int) (*str1 - *str2);						// return the int casted value of *str1 - *str2.
}


// my_strrev : Takes a string argment, reverse it, and return the starting address of the modifed string.
char* my_strrev(char *dest){
	char* sp = dest;	// Pointer at the beginning of the string.
	char* ep = dest;	// Pointer at the (will-be) end of the string. 
	char temp;
	while (*ep){
		ep++;			// Move ep to the end of the string.
	}
	ep--;				// Move ep to the last non-null character of the string.
	while (sp < ep){	// Repeat this until sp or ep reaches the mid point.
		temp = *sp;		// Swap char value at sp and ep.
		*sp = *ep;		// Swap char value at sp and ep.
		*ep = temp;		// Swap char value at sp and ep.
		sp++;			// Increment sp
		ep--;			// Increment ep
	}
	return dest;		// Return the pointer of the modified string.
}

// my_strrev : Takes a string argment, convert the cases, and return the starting address of the modifed string.
char* my_strccase(char *dest){
	char* cp = dest;							// Copy dest into cp.
	int adjuster = 'a' - 'A';					// ASCII value diff between the upper and the lower case alphabets.
	while (*cp){								// Shift until the end of the string.
		if (*cp >= 'A' && *cp <= 'Z'){		// If current char is the upper case alphabet.
			*cp += adjuster;					// Modify into lower case.
		} else if (*cp >= 'a' && *cp <= 'z'){	// If current char is the lower case alphabet.
			*cp -= adjuster;					// Modify into upper case.
		}
		cp++;									// Increment cp.
	}
	return dest;								// Return the pointer of the modified string.
}

// my_strtok : Takes a string and a delimiter string, and returns a pointer to the first token found in the string. 
//             A null pointer is returned if there are no tokens left to retrieve.
char* my_strtok(char *str, const char* delim){
	static char* last;			// last declared as static so that it can be used in the next call with str=NULL.

	if (str != NULL){
		last = str;				// Initial call with str : Assign last = str.
	} else if (*last){
		str = last;				// Call with str=NULL and the static variable last has not reached '\0'.
	}

	while (*last){						// While the token has not reached the end of the string.
		if (my_strchr(delim, *last)){	// Use my_strchr to check if *last is contained in delim.
			*last = '\0';				// If so, overwrite '\0' on that position and tokenize the string.
			last++;						//        make static variable last to point at the next position.
			return str;					//        return the starting point of the current token.
		}
		last++;							// If not, increment last and check the next character.
	}
	return str;							// If nothing was found, return either the starting address of the current string,
}										//                              or the NULL pointer if last reached '\0'.