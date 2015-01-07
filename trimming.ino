void trim_and_split () {
    inputString.trim(); //The variable.trim() command cuts out any displayable characters
    display1 = inputString.substring(0,16); //This splits the text into four variables
    display2 = inputString.substring(16,32); //A variable for each line
    display3 = inputString.substring(32,48);
    display4 = inputString.substring(48,64);
}
