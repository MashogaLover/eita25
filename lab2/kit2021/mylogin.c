/*
 * Shows user info from local pwfile.
 *
 * Usage: userinfo username
 */

#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "pwdblib.h"

/* Define some constants. */
#define USERNAME_SIZE (32)
#define NOUSER (-1)


int print_info(const char *username)
{
  while(1) {
    struct pwdb_passwd *p = pwdb_getpwnam(username);
    char *password = getpass("password: ");
    if (p != NULL) {
      char salt[2];
      salt[0] = p->pw_passwd[0];
      salt[1] = p->pw_passwd[1];
      if(strcmp(p->pw_passwd,crypt(password,salt)) == 0){
        printf("User authenticated successfully\n");
        return 0;
      }
    }
    printf("Unknown user or incorrect password\n");
    read_username(username);
  }
  return 0;
}

void read_username(char *username)
{
  printf("login: ");
  fgets(username, USERNAME_SIZE, stdin);
  /* remove the newline included by getline() */
  username[strlen(username) - 1] = '\0';
}

int main(int argc, char **argv)
{
  char username[USERNAME_SIZE];

  /*
   * Write "login: " and read user input. Copies the username to the
   * username variable.
   */
  read_username(username);

  /* Show user info from our local pwfile. */
  if (print_info(username) == NOUSER) {
      /* if there are no user with that usename... */
      printf("\nFound no user with name: %s\n", username);
      return 0;
  }
}
