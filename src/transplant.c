#include "const.h"
#include "transplant.h"
#include "debug.h"
#include <stdlib.h> //I added this
#include <dirent.h> //I added this
#include <errno.h>  //I added this

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the pathname of the current file or directory
 * as well as other data have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

/*
 * A function that returns printable names for the record types, for use in
 * generating debugging printout.
 */
static char *record_type_name(int i) {
    switch(i) {
    case START_OF_TRANSMISSION:
	return "START_OF_TRANSMISSION";
    case END_OF_TRANSMISSION:
	return "END_OF_TRANSMISSION";
    case START_OF_DIRECTORY:
	return "START_OF_DIRECTORY";
    case END_OF_DIRECTORY:
	return "END_OF_DIRECTORY";
    case DIRECTORY_ENTRY:
	return "DIRECTORY_ENTRY";
    case FILE_DATA:
	return "FILE_DATA";
    default:
	return "UNKNOWN";
    }
}

/*
 * @brief  Initialize path_buf to a specified base path.
 * @details  This function copies its null-terminated argument string into
 * path_buf, including its terminating null byte.
 * The function fails if the argument string, including the terminating
 * null byte, is longer than the size of path_buf.  The path_length variable 
 * is set to the length of the string in path_buf, not including the terminating
 * null byte.
 *
 * @param  Pathname to be copied into path_buf.
 * @return 0 on success, -1 in case of error
 */
int path_init(char *name) {             //should path start with /
    //printf("path from validargs: %s\n", name);
    //store name into path_buf
    int i = 0;
    char c;
    for(c = 0; *(name + c) != '\0'; ++c){
        *(path_buf + c) = *(name + c);
        i++;
    }
    *(path_buf + c) = '\0';
    //printf("path init: %s\n", path_buf);
    //check to make sure name isnt longer then the size of path_buf (how should this be done??????????)
    //printf("length of path(i): %d\n", i);
    //set path_length to size of path_buf will null terminator
    path_length = i + 1;
    //printf("path length var: %d\n", path_length);
    return 0; //-1 if error
}

/*
 * @brief  Append an additional component to the end of the pathname in path_buf.
 * @details  This function assumes that path_buf has been initialized to a valid
 * string.  It appends to the existing string the path separator character '/',
 * followed by the string given as argument, including its terminating null byte.
 * The length of the new string, including the terminating null byte, must be
 * no more than the size of path_buf.  The variable path_length is updated to
 * remain consistent with the length of the string in path_buf.
 * 
 * @param  The string to be appended to the path in path_buf.  The string must
 * not contain any occurrences of the path separator character '/'.
 * @return 0 in case of success, -1 otherwise.
 */
int path_push(char *name) {                 //do more testing***
    int end_path = path_length - 1;     //accounts for null terminator
    *(path_buf + end_path) = '/';       //adds the path seperator  
    end_path = end_path + 1;            //accounts for path seperator
    //printf("end path: %d\n", end_path);
    int c = 0;
    int cnt = 0; 
    while(*(name + cnt) != '\0'){
        cnt++;
    }
    //printf("cnt: %d\n", cnt);
    //printf("name: %s\n", name);
    while(c < cnt){
        *(path_buf + end_path) = *(name + c);
        //printf("c in loop: %d\n", c);
        //printf("end path in loop: %d\n", end_path);
        c++;
        end_path++;
    }
    *(path_buf + end_path) = '\0';             //adds null terminator at the end 
    //printf("path push: %s\n", path_buf);
    path_length = end_path + 1;                     //update path_length
    //printf("path length var: %d\n", path_length);
    return 0; //return -1 for falure 
}

/*
 * @brief  Remove the last component from the end of the pathname.
 * @details  This function assumes that path_buf contains a non-empty string.
 * It removes the suffix of this string that starts at the last occurrence
 * of the path separator character '/'.  If there is no such occurrence,
 * then the entire string is removed, leaving an empty string in path_buf.
 * The variable path_length is updated to remain consistent with the length
 * of the string in path_buf.  The function fails if path_buf is originally
 * empty, so that there is no path component to be removed.
 *
 * @return 0 in case of success, -1 otherwise.
 */
int path_pop() {        //do more testing*** 
    if(path_buf == NULL){
        return -1;
    }
    int i = path_length - 1;
    while(i--){
        if(*(path_buf + i) == '/' || *(path_buf + i) == '\0'){
            break;
        } else {
            *(path_buf + i) = '\0';
        }
    }
    //set the last character also to null if its a /
    //printf("path_buf: %c\n", *(path_buf + i));
    if(*(path_buf + i) == '/'){
       *(path_buf + i) = '\0'; 
    }
    //update path_length
    int cnt = 0; 
    while(*(path_buf + cnt) != '\0'){
        cnt++;
    }
    //printf("size of counter: %d\n", cnt);
    path_length = cnt + 1;
    //printf("size of path length: %d\n", path_length);
    //printf("path pop: %s\n", path_buf);
    return 0;   
}

/*
 * my helper functions 
 */

int string_compare(char* str1, char* str2){
    int ctr = 0;
    if(str1 == NULL){
        return 0;
    }
    while(*(str1 + ctr) == *(str2 + ctr)){
        if(*(str1 + ctr) == '\0'|| *(str2 + ctr) == '\0')
            break;
        ctr++;
    }
    if(*(str1 + ctr) == '\0' && *(str2 + ctr) == '\0')
        return 0;
    else
        return -1;
}

int getFilename(){
    int i = path_length - 1;
    int j = 0;
    while(i--){
        if(*(path_buf + i) == '/' || *(path_buf + i) == '\0'){
            break;
        } else {
            j++;
        }
    }
    //printf("j: %d\n", j);
    //printf("path_length: %d\n", path_length);
    int start = path_length - j - 1;
    for(int k = 0; k < path_length; k++){
        *(name_buf + k) = *(path_buf + start);
        start++;
    }
    //printf("getFilename: %s\n", name_buf);
    //printf("size of name: %d\n", j);
    return j;       //return the size of the filename
}

int getFilenameDS(int len){
    char c;
    int i = 0;
    for(i = 0; i < len; i++){
        c = getchar();
        *(name_buf + i) = c; 
        //printf("i: %d\n", i);
    }
    *(name_buf + i) = '\0';
    return 0;
}

int getMagic(){
    int magic = 0;
    int first = getchar();
    int second = getchar();
    int third = getchar();
    first = first << 16;
    second = second << 8;
    magic = first + second + third;
    //printf("Magic: %X\n", magic);
    return magic;
}

int getType(){
    int type = 0;
    int fourth = getchar();
    type = fourth;
    //printf("Type: %X\n", type);
    return type;
}

int getDepth(){
    int depth = 0;
    int fifth = getchar();
    int sixth = getchar();
    int seventh = getchar();
    int eighth = getchar();
    fifth = fifth << 24;
    sixth = sixth << 16;
    seventh = seventh << 8;
    depth = fifth + sixth + seventh + eighth;
    //printf("Depth: %X\n", depth);
    return depth;
}

int getSize(){
    int size = 0;
    long long ninth = getchar();
    long long tenth = getchar();
    long long eleventh = getchar();
    long long twelveth = getchar();
    int thirteenth = getchar();
    int fourteenth = getchar();
    int fiftheenth = getchar();
    int sixteenth = getchar();
    ninth = ninth << 56;
    tenth = tenth << 48;
    eleventh = eleventh << 40;
    twelveth = twelveth << 32;
    thirteenth = thirteenth << 24;
    fourteenth = fourteenth << 16;
    fiftheenth = fiftheenth << 8;
    size = ninth + tenth + eleventh + twelveth + thirteenth + fourteenth + fiftheenth + sixteenth;
    //printf("Size: %X\n", size);
    return size;
}

int printMagic(){
    int magic = 0x0c0ded;
    int first = (magic >> 16);
    int second = (magic & 0x0000ff00);
    second = second >> 8;
    int third = (magic & 0x000000ff);
    
    putchar(first);
    putchar(second);
    putchar(third);
    return 0;
}

int printType(int type){
    int byte = (type & 0x000000ff);
    
    putchar(byte);
    return 0;
}    

int printDepth(int depth){
    int first = (depth >> 24);
    int second = (depth & 0x00ff0000);
    second = second >> 16;
    int third = (depth & 0x0000ff00);
    third = third >> 8;
    int fourth = (depth & 0x000000ff);

    putchar(first);
    putchar(second);
    putchar(third);
    putchar(fourth);
    return 0;
}

int printSize(long long size){
    long long first = (size >> 56);
    long long second = (size & 0x00ff000000000000);
    second = second >> 48;
    long long third = (size & 0x0000ff0000000000);
    third = third >> 40;
    long long fourth = (size & 0x000000ff00000000);
    fourth = fourth >> 32;
    long long fifth = (size & 0x00000000ff000000);
    fifth = fifth >> 24;
    long long sixth = (size & 0x0000000000ff0000);
    sixth = sixth >> 16;
    long long seventh = (size & 0x000000000000ff00);
    seventh = seventh >> 8;
    long long eighth = (size & 0x00000000000000ff);

    putchar(first);
    putchar(second);
    putchar(third);
    putchar(fourth);
    putchar(fifth);
    putchar(sixth);
    putchar(seventh);
    putchar(eighth);
    return 0;
}

/*
 * @brief Deserialize directory contents into an existing directory.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory.  It reads (from the standard input) a sequence of DIRECTORY_ENTRY
 * records bracketed by a START_OF_DIRECTORY and END_OF_DIRECTORY record at the
 * same depth and it recreates the entries, leaving the deserialized files and
 * directories within the directory named by path_buf.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * each of the records processed.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including depth fields in the records read that do not match the
 * expected value, the records to be processed to not being with START_OF_DIRECTORY
 * or end with END_OF_DIRECTORY, or an I/O error occurs either while reading
 * the records from the standard input or in creating deserialized files and
 * directories.
 */
int deserialize_directory(int depth) {
    // To be implemented. (deserialization: turn a sequence of bytes into the directory) create directory
    int myType = 0;
    while(myType != 1){
        int myMagic = getMagic();
        if(myMagic != 0x0c0ded){
            //printf("error in magic deserialize_directory\n");
            return -1;
        }
        myType = getType();
        int myDepth = getDepth();
        if(myDepth == depth){
            //use current directory
        } 
        int mySize = getSize();
        if(myType == 2){
            //printf("start of directory\n");
        } else if(myType == 4){
            //printf("directory entry\n");
            //read metadata
            int myPerm = getDepth();
            int deSize = getSize();
            //printf("metadata from dir entry\n");
            //get directory or file name 
            int filenameLen = mySize - 28;      //subtract both headers
            getFilenameDS(filenameLen);
            //printf("filename (name_buf): %s\n", name_buf);
            path_push(name_buf);
            //printf("path_buf: %s\n", path_buf);
            //check if dir exists if not create one
            if(S_ISDIR(myPerm)){
                //printf("is a directory\n");
                DIR* dir = opendir(path_buf);
                if(dir){
                    //directory exists
                    //printf("directory exists, no need to make one\n");
                    //call deserialize_directory then that calls deserialize_file
                    closedir(dir);
                } else if(ENOENT == errno){
                    //directory doesnt exist
                    //printf("directory doesnt exist, make one\n");
                    int check;
                    check = mkdir(path_buf, 0700);      //0700: created with read, write and execute permissions change this later****
                    if(!check){
                        //directory is created
                        //printf("new directory is created\n");
                    } else {
                        //unable to create directory
                        //printf("could not create directory\n");
                        return -1;
                    }
                } else{
                    //other error
                    //printf("other error occured opening path_buf\n");
                    return -1;
                }
            }
            //check and change permissions of the directory
            chmod(path_buf, myPerm & 0777);
        } else if(myType == 5){
            //printf("file data\n");
            if(deserialize_file(myDepth) != 0){
                return -1;
            }
            path_pop();
        } else if (myType == 1){
            //printf("end of transmission\n");
            return 0;
        } else if (myType == 3){
            //printf("end of directory\n");
            path_pop();
        }
    }
    return 0;
}

/*
 * @brief Deserialize the contents of a single file.
 * @details  This function assumes that path_buf contains the name of a file
 * to be deserialized.  The file must not already exist, unless the ``clobber''
 * bit is set in the global_options variable.  It reads (from the standard input)
 * a single FILE_DATA record containing the file content and it recreates the file
 * from the content.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * the FILE_DATA record.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including a depth field in the FILE_DATA record that does not match
 * the expected value, the record read is not a FILE_DATA record, the file to
 * be created already exists, or an I/O error occurs either while reading
 * the FILE_DATA record from the standard input or while re-creating the
 * deserialized file.
 */
int deserialize_file(int depth){
    //create file, check to see if it exists********
    struct stat stat_buf;
    if(stat(path_buf, &stat_buf) != -1 && global_options == 12){
        //file does exist and clobber is set
        // delete file
        int status = remove(path_buf);
        if(status == 0){
            //printf("file was deleted successfully\n");
        } else {
            //printf("unable to delete file\n");
            return -1;
        }
        //create file
        FILE *f;
        f = fopen(path_buf, "w");
        if(f == NULL){
            //printf("file could not be created\n");
            return -1;
        }
        char c = getchar();
        while(c != '\n'){
            fputc(c, f);
            //printf("char DF: %c\n", c);
            c = getchar();
        }
        fclose(f);
        //check and change permissions of the file | dont have access to myPerm
        //chmod(path_buf, myPerm & 0777);
    } else if(stat(path_buf, &stat_buf) != -1 && global_options != 12){
        //file does exist and clobber is not set
        //printf("file does exist and clobber is not set returning -1\n");
        return -1;
    } else {
        FILE *f;
        f = fopen(path_buf, "w");
        if(f == NULL){
            //printf("file could not be created\n");
            return -1;
        }
        char c = getchar();
        while(c != '\n'){
            fputc(c, f);
            //printf("char DF: %c\n", c);
            c = getchar();
        }
        fclose(f);
    }
    return 0;
}

/*
 * @brief  Serialize the contents of a directory as a sequence of records written
 * to the standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory to be serialized.  It serializes the contents of that directory as a
 * sequence of records that begins with a START_OF_DIRECTORY record, ends with an
 * END_OF_DIRECTORY record, and with the intervening records all of type DIRECTORY_ENTRY.
 *
 * @param depth  The value of the depth field that is expected to occur in the
 * START_OF_DIRECTORY, DIRECTORY_ENTRY, and END_OF_DIRECTORY records processed.
 * Note that this depth pertains only to the "top-level" records in the sequence:
 * DIRECTORY_ENTRY records may be recursively followed by similar sequence of
 * records describing sub-directories at a greater depth.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open files, failure to traverse directories, and I/O errors
 * that occur while reading file content and writing to standard output.
 */
//"Magic" (3 bytes): 0x0C, 0x0D, 0xED -> Type (1 byte) -> Depth (4 bytes) -> Size (8 bytes)
int serialize_directory(int depth) {
    //(turn a directory into a sequence of bytes)
    //print start of directory
    printMagic();                                          
    printType(0x02);
    printDepth(depth);
    printSize(0x10);       
    //traverse file system
    struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(path_buf);
    if(dr == NULL){         //check if dir is empty
        //printf("Directory is empty\n");
        return -1;
    }
    while ((de = readdir(dr)) != NULL){ 
        //use de -> d_name to access the directories
        if(string_compare(de -> d_name, ".") == 0 ){
            //printf("path is .\n");            //what should i do with this
        } else if(string_compare(de -> d_name, "..") == 0){
            //printf("path is ..\n");           //what should i do with this
        } else {
            path_push(de -> d_name);
            //printf("*****Path from serialize_directory: %s\n", path_buf);
            struct stat stat_buf;
            if(stat(path_buf, &stat_buf) != 0){
                return -1;
            }
            if(S_ISDIR(stat_buf.st_mode)){
                //printf("is a directory\n");
                // directory entry 
                printMagic();                                    
                printType(0x04);
                printDepth(depth);
                //printSize();                //16 + 12 + filename size (decimal)
                int cnt = getFilename();
                int cnt1 = cnt + 28;
                printSize(cnt1);
                // metadata
                printDepth(stat_buf.st_mode);
                printSize(stat_buf.st_size);
                //putchar() filename or directory name
                for(int i = 0; i < cnt; i++){
                    putchar(*(name_buf + i));
                }
                //recursive call
                serialize_directory(depth + 1);
            } 
            if(S_ISREG(stat_buf.st_mode)){
                //printf("is a file\n");
                //directory entry 
                printMagic();                                    
                printType(0x04);
                printDepth(depth);
                //printSize();                //16 + 12 + filename size (decimal)
                int cnt = getFilename();
                int cnt1 = cnt + 28;
                printSize(cnt1);
                //metadata
                printDepth(stat_buf.st_mode);
                printSize(stat_buf.st_size);
                //putchar() filename or directory name
                for(int i = 0; i < cnt; i++){
                    putchar(*(name_buf + i));
                }
                //call serialize file
                serialize_file(depth, stat_buf.st_size);
            }
            path_pop();
        }
    }
    closedir(dr);
    // end of directory
    printMagic();                                             
    printType(0x03);
    printDepth(depth);
    printSize(0x10);    
    return 0;
}

/*
 * @brief  Serialize the contents of a file as a single record written to the
 * standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * file to be serialized.  It serializes the contents of that file as a single
 * FILE_DATA record emitted to the standard output.
 *
 * @param depth  The value to be used in the depth field of the FILE_DATA record.
 * @param size  The number of bytes of data in the file to be serialized.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open the file, too many or not enough data bytes read
 * from the file, and I/O errors reading the file data or writing to standard output.
 */
//"Magic" (3 bytes): 0x0C, 0x0D, 0xED -> Type (1 byte) -> Depth (4 bytes) -> Size (8 bytes)
int serialize_file(int depth, off_t size) {
    //print file data record header
    struct stat stat_buf;
    if(stat(path_buf, &stat_buf) != 0){
        return -1;
    }
    //printf("Size from stat_buf reg Serialize: %ld\n", stat_buf.st_size);
    //file data
    printMagic();                                           
    printType(0x05);
    printDepth(depth);
    int s = 0x10 + stat_buf.st_size;
    printSize(s);                    //16 + size of contents of file 
    //prints contents of a file
    FILE *f;
    f = fopen(path_buf, "r");
    if(f == NULL){
        //file cant be read
        return -1;
    }
    char c;
    c = fgetc(f);
    while(c != EOF){
       putchar(c);
       c = fgetc(f); 
    }
    fclose(f);
    return 0;
}

/**
 * @brief Serializes a tree of files and directories, writes
 * serialized data to standard output.
 * @details This function assumes path_buf has been initialized with the pathname
 * of a directory whose contents are to be serialized.  It traverses the tree of
 * files and directories contained in this directory (not including the directory
 * itself) and it emits on the standard output a sequence of bytes from which the
 * tree can be reconstructed.  Options that modify the behavior are obtained from
 * the global_options variable.
 *
 * @return 0 if serialization completes without error, -1 if an error occurs.
 */
//"Magic" (3 bytes): 0x0C, 0x0D, 0xED -> Type (1 byte) -> Depth (4 bytes) -> Size (8 bytes)
int serialize() {
    //start of transmission
    printMagic();                                            
    printType(0x00);
    printDepth(0x00);
    printSize(0x10);
    //call serialize directory
    serialize_directory(1);
    //end of transmission
    printMagic();                                               
    printType(0x01);
    printDepth(0x00);
    printSize(0x10);
    //printf("\n");
    return 0;
}

/**
 * @brief Reads serialized data from the standard input and reconstructs from it
 * a tree of files and directories.
 * @details  This function assumes path_buf has been initialized with the pathname
 * of a directory into which a tree of files and directories is to be placed.
 * If the directory does not already exist, it is created.  The function then reads
 * from from the standard input a sequence of bytes that represent a serialized tree
 * of files and directories in the format written by serialize() and it reconstructs
 * the tree within the specified directory.  Options that modify the behavior are
 * obtained from the global_options variable.
 *
 * @return 0 if deserialization completes without error, -1 if an error occurs.
 */
int deserialize() {
    //check to see if directory exists
    DIR* dir = opendir(path_buf);
    if(dir){
        //directory exists | dont do anything with clobber
        //printf("directory exists, no need to make one\n");
        closedir(dir);
        //return or dont the directions are unclear
    } else if(ENOENT == errno){
        //directory doesnt exist | clobber does not effect this
        //printf("directory doesnt exist, make one\n");
        int check;
        check = mkdir(path_buf, 0700);      //0700: created with read, write and execute permissions change this later****
        if(!check){
            //directory is created
            //printf("new directory is created\n");
        } else {
            //unable to create directory
            //printf("could not create directory\n");
            return -1;
        }
    } else{
        //other error
        //printf("other error occured opening path_buf\n");
        return -1;
    }
    //read from stdin
    int myMagic = getMagic();
    if(myMagic != 0x0c0ded){
        //printf("error in magic deserialize\n");
        return -1;
    }
    int myType = getType();
    if(myType != 0){
        //printf("error in type Deserialize\n");
        return -1;
    }
    int myDepth = getDepth();
    getSize();
    //printf("start of transmission\n");
    //check and change permissions of the directory | dont think i need to do this here
    //chmod(path_buf, myPerm & 0777);
    deserialize_directory(myDepth);
    return 0;
}


/*
 * my helper functions
 */
char validateFlag(char* str){
    //check for leading "-"
    if(*str != '-'){
        return -1;
    }

    //check for valid str length 
    char character = *(str);
    int cnt = 0;
    while(character != 0){
        //printf("character: %c\n", character);
        if(cnt > 1){
            return -1;
        }
        cnt++;
        character = *(str + cnt);
    }

    //check if theres a letter after '-'
    if(cnt < 2){
        return -1;
    }
    //printf("counter: %d\n", cnt);

    //check for valid letter
    //printf("String: %s\n", str);
    char vflag = *(str + 1);
    if(vflag == 'h'){
        return vflag;
    } else if(vflag == 's'){
        return vflag;
    } else if(vflag == 'd'){
        return vflag;
    } else {
        return -1;
    }
}

char validateSecFlag(char* secStr){
    //check for leading '-'
    if(*secStr != '-'){
        return -1;
    }

    //check for valid string len
    char secCharacter = *(secStr);
    int secCnt = 0;
    while(secCharacter != 0){
        //printf("sec character: %c\n", secCharacter);
        if(secCnt > 1){
            return -1;
        }
        secCnt++;
        secCharacter = *(secStr + secCnt);
    }

    //check if theres a letter after '-'
    if(secCnt < 2){
        return -1;
    }
    //printf("sec counter: %d\n", secCnt);

    //printf("String: %s\n", secStr);
    char yflag = *(secStr + 1);
    if(yflag == 'c'){
        return yflag;
    } else if (yflag == 'p'){
        return yflag;
    } else {
        return -1;
    }
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv){
    //reset global options each time???? 

    //checks for no arguments
    if (argc == 1){
        global_options = 0;
        return -1;
    }

    //get first argument
    char* firstArg = *(argv + 1);
    //printf("First arg: %s\n", firstArg);

    //check to make sure firstArg is valid, validateFlag will return -1 on error and the apporpriate letter on success
    char flag = validateFlag(firstArg);
    //printf("current flag is: %c\n", flag); 

    //checks if vaidateFlag returns an error
    if(flag == -1){
        //printf("*******************invalid flag: %c\n", flag);
        global_options = 0;
        return -1;
    }  

    //handle first arg has to be either h s or d
    if(flag == 'h'){
        //if the first arg is h nothing else after matters (this is handled)
        //display usage 
        global_options = (global_options | 0x1);
        return 0;
    } else if (flag == 's'){
        global_options = (global_options | 0x2);
    } else if (flag == 'd'){
        global_options = (global_options | 0x4);
    } else {
        global_options = 0;
        return -1;
    }

    //printf("Global options variable: %d\n", global_options);

    char* secondArg = *(argv + 2);
    //printf("Second arg: %s\n", secondArg);
    //could be c or p, if its p needs to be followed by pathname 

    if(secondArg != NULL){
        //check to make sure secondArg is valid, validateSecFlag will return -1 on error and the apporpriate letter on success
        char secFlag = validateSecFlag(secondArg);

        //checks if vaidateSecFlag returns an error
        if(secFlag == -1){
            //printf("*******************invalid sec flag: %c\n", secFlag);
            global_options = 0;
            return -1;
        }

        if(secFlag == 'c'){
            //if the first flag is -s return EXIT_FAILURE
            if(flag == 's'){
                global_options = 0;
                //printf("Flag: %c\n", flag);
                return -1;
            }
            global_options = (global_options | 0x8);
            //check for -p and then a vaild path 
            char* lastFlagC = *(argv + 3);
            char* pFlag = "-p";
            if(lastFlagC != NULL){
                if(string_compare(lastFlagC, pFlag) == 0){
                    //printf("lastFlagC: %s\n", lastFlagC);
                    //handle path needs to be handled 
                    char* pathFromC = *(argv + 4);
                    if(pathFromC == NULL){
                        global_options = 0;
                        return -1;
                    }
                    //printf("Path from c: %s\n", pathFromC);
                    //call path_init on pathFromC
                    path_init(pathFromC);
                } else {
                    global_options = 0;
                    return -1;
                }
            }
        }else if(secFlag == 'p'){
            //check for path
            char* path = *(argv + 3);
            //printf("path: %s\n", path);
            if(path == NULL){  
                global_options = 0;
                return -1;
            } else if(*path == '-'){
                printf("failed\n");
                global_options = 0;
                return -1;
            } else {
                //handle path
                //printf("valid path: %s\n", path);
                //call path_init on path
                path_init(path);
                char* lastFlagP = *(argv + 4);
                if(string_compare(lastFlagP, "-c") != 0){
                    global_options = 0;
                    return -1;
                } else if(lastFlagP == NULL){
                    //continue;
                } else {
                    //printf("lastFlagP: %s\n", lastFlagP);
                    //printf("no seg fault bottom*****\n");
                    global_options = (global_options | 0x8);
                }
            }
        }else{
            global_options = 0;
            return -1;
        }
    }
    //handles if there is no path passed in 
    if(string_compare(firstArg, "-s") == 0 && secondArg == NULL){
        path_init("./");
    } 

    //printf("the end: %d\n", global_options);       
    return 0; //was -1
}

