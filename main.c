#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
  int id;
  char * uniqueName; // Mutable pointer to mutable characters.
  char * displayName;
} Player; 

Player* createPlayer(int const aId, char const * aUniqueName, char const * aDisplayName) {
  Player* pNewPlayer = (Player*)malloc(sizeof(Player));
  pNewPlayer->id = aId;
  pNewPlayer->uniqueName = (char *)malloc(1024 * sizeof(char));
  memcpy(pNewPlayer->uniqueName, aUniqueName, strlen(aUniqueName) * sizeof(char));
  pNewPlayer->displayName = (char *)malloc(1024 * sizeof(char));
  memcpy(pNewPlayer->displayName, aDisplayName, strlen(aDisplayName) * sizeof(char));
  return pNewPlayer; 
}

void printDashline() {
  printf("\n---------------------------\n");
}

void printPlayer(Player* pPlayer) {
  printf("\nPlayer: {\n\tid: %d,\n\tuniqueName: %s,\n\tdisplayName: %s\n}\n", pPlayer->id, pPlayer->uniqueName, pPlayer->displayName);
}

void destroyPlayer(Player* pPlayer) {
  if (!pPlayer) return;
  if (NULL != pPlayer->uniqueName) free(pPlayer->uniqueName);
  if (NULL != pPlayer->displayName) free(pPlayer->displayName);
  free(pPlayer);
}

int main() {
  int* p = (int*)malloc(5 * sizeof(int));
  printDashline();
  printf("Address of variable p in stack is %p, address of the data pointed to by variable p in heap is %p.\n", &p, p);
  free(p);

  printDashline();
  Player* pFoo = createPlayer(42, "PC1667", "FarmerJohn");
  printPlayer(pFoo);
  printf("\nAddress of variable pFoo in StackRAM is %p with size %lu, address of the data pointed to by variable pFoo in heap is %p.\n", &pFoo, sizeof(pFoo), pFoo);

  Player foo = *pFoo;
  printf("\nAddress of variable foo in StackRAM is %p with size %lu, same as sizeof(Player) == %lu.\n", &foo, sizeof(foo), sizeof(Player));

  printDashline();
  printf("\nAssigning new values to the fields of foo only takes effect in StackRAM.\n");
  foo.id = 33;
  foo.uniqueName = "PC1009";
  foo.displayName = "RadioSam";
  printPlayer(&foo);

  printf("\nThe fields of (*pFoo) in HeapRAM remains unchanged.\n");
  printPlayer(pFoo);

  printDashline();
  printf("\nHowever there're ways to change the fields of (*pFoo) in HeapRAM.\n");
  (*pFoo).id = 77;
  char const * const newUniqueName = "PC2008"; // Immutable pointer to immutable characters. 
  memcpy((*pFoo).uniqueName, newUniqueName, strlen(newUniqueName) * sizeof(char));
  char const * newDisplayName = "WarriorJohn"; // Mutable pointer to immutable characters.
  memcpy(pFoo->displayName, newDisplayName, strlen(newDisplayName) * sizeof(char));
  /**
  * Don't use  
  *
  * `(*pFoo).uniqueName = "PC2008";`, or 
  * `(*pFoo).uniqueName = newUniqueName;`, or 
  * `pFoo->displayName = "WarriorJohn";`, or 
  * `pFoo->displayName = newDisplayName;` 
  *
  * which will result in leak of HeapRAM. 
  */
  printPlayer(pFoo);

  destroyPlayer(pFoo);
  printDashline();
  printf("\nThe occupied space of (*pFoo) in HeapRAM is deallocated, exiting the current process.\n");
  return 0;
}
