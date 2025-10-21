#ifndef dotenv_h
#define dotenv_h

char *env(char *variable);
void  dotenv(char *path);

void  dotenvClean();

#endif