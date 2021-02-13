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
  struct pwdb_passwd *oldp = pwdb_getpwnam(username);

  while(1) {
    struct pwdb_passwd *p = pwdb_getpwnam(username);
    char *password = getpass("password: ");
    if (p != NULL) {
      if(oldp->pw_failed < 5) {
        char salt[2];
        salt[0] = p->pw_passwd[0];
        salt[1] = p->pw_passwd[1];
        if(strcmp(p->pw_passwd,crypt(password,salt)) == 0){
          printf("User authenticated successfully\n");

          oldp->pw_age=oldp->pw_age+1;
          oldp->pw_failed=0;

          printf("%d\n", oldp->pw_age);
          if(oldp->pw_age >= 10) {
            printf("%s\n", "Change passsword idiot");
          }
        } else {
          printf("Unknown user or incorrect password\n");

          if(strcmp(p->pw_name, oldp->pw_name) != 0) {
            oldp = pwdb_getpwnam(username);
          }
          p->pw_failed=oldp->pw_failed+1;
          *oldp=*p;
        }
      } else {
        printf("%s\n", "VAC banned on csgo");
      }
    } else {
      printf("Unknown user or incorrect password\n");
    }

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
