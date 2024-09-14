
int strlen (char *s) {
    int i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

void append_str (char *s, char ch) {
    int i = strlen(s);
    s[i] = ch; // append ch at the tail of string
    s[i + 1] = 0; // append empty character
}

void pop_str (char *s) {
    int i = strlen(s);
    s[i - 1] = 0; // pop the last character
}

// \return 0 when x y is identical
int strcmp (const char *X, const char *Y) {
    while (*X) {
        if (*X != *Y) {break;}
        X++;
        Y++;
    }
    return *(const unsigned char*)X - *(const unsigned char*)Y;
    
}