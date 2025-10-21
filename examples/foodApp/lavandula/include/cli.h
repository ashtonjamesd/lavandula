#ifndef cli_h
#define cli_h

int newProject(char *name);
int runProject();
int buildProject();
int migrate();

int help();
int version();
int unknownCommand(char *option);

#endif