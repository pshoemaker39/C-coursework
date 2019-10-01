//Price Shoemaker - Systems II - DePaul University

/*-------------------------------------------------------------------------*
 *---									---*
 *---		sandwichMaker.cpp					---*
 *---									---*
 *---	    This program uses a C-implemented linked list to keep track	---*
 *---	of multiple Sandwich orders, submitted by multiple ordering	---*
 *---	thread and fulfilled by multiple sandwich-making threads.	---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2019 February 21	Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/
//----			Necessary inclusions:				----//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> // For thread-related functions
#include <unistd.h>  // For sleep()

//----			Definitions of constants:			----//

//  PURPOSE:  To tell the necessary number of command line arguments.
const int NUM_NECESSARY_CMD_LINE_ARGS = 5;

//  PURPOSE:  To tell the index of the buffer length argument on the command
//	line.
const int BUFFER_LEN_CMD_LINE_INDEX = 1;

//  PURPOSE:  To tell the index of the number of makers argument on the command
//	line.
const int NUM_MAKERS_CMD_LINE_INDEX = 2;

//  PURPOSE:  To tell the index of the number of orderers argument on the
//	command line.
const int NUM_ORDERERS_CMD_LINE_INDEX = 3;

//  PURPOSE:  To tell the index of the number of sandwiches argument on the
//	command line.
const int NUM_SANDWICHES_CMD_LINE_INDEX = 4;

//  PURPOSE:  To tell the size of variables of type 'const char*' in bytes.
const int SIZEOF_CPTR = sizeof(const char *);

//  PURPOSE:  To hold the names of the varieties of bread that may be used
//	for the sandwiches.
const char *BREAD_NAME_ARRAY[] = {
    "white bread",
    "wheat bread",
    "pita bread",
    "pumpernickel bread",
    "a sesame bun",
    "last quarter's final exams"};

//  PURPOSE:  To tell how many options are in 'BREAD_NAME_ARRAY[]'.
const int NUM_BREADS = sizeof(BREAD_NAME_ARRAY) / SIZEOF_CPTR;

//  PURPOSE:  To hold the names of the varieties of primary toppings that may
//	be used for the sandwiches.
const char *MAIN_TOPPING_ARRAY[] = {
    "peanut butter",
    "grilled tofu",
    "salami",
    "falafel",
    "nutella",
    "squirmy earthworms"};

//  PURPOSE:  To tell how many options are in 'MAIN_TOPPING_ARRAY[]'.
const int NUM_MAIN_TOPPINGS = sizeof(MAIN_TOPPING_ARRAY) / SIZEOF_CPTR;

//  PURPOSE:  To hold the names of the varieties of secondary toppings that may
//	be used for the sandwiches.
const char *SECOND_TOPPING_ARRAY[] = {
    "jelly",
    "cheddar cheese",
    "honey",
    "lettuce",
    "tomato",
    "flamin' hot cheetos"};

//  PURPOSE:  To tell how many options are in 'SECOND_TOPPING_ARRAY[]'.
const int NUM_SECOND_TOPPINGS = sizeof(SECOND_TOPPING_ARRAY) / SIZEOF_CPTR;

//  PURPOSE:  To tell the minimum length of the order buffer.
const int MIN_BUFFER_LEN = 1;

//  PURPOSE:  To tell the minimum length of the order buffer.
const int MAX_BUFFER_LEN = 10;

//  PURPOSE:  To tell the minimum number of sandwiches makers.
const int MIN_NUM_MAKERS = 1;

//  PURPOSE:  To tell the maximum number of sandwiches makers.
const int MAX_NUM_MAKERS = 4;

//  PURPOSE:  To tell the minimum number of sandwiches orderers.
const int MIN_NUM_ORDERERS = MIN_NUM_MAKERS;

//  PURPOSE:  To tell the maximum number of sandwiches orderers.
const int MAX_NUM_ORDERERS = MAX_NUM_MAKERS;

//  PURPOSE:  To tell the minimum total number of sandwiches to make.
const int MIN_NUM_SANDWICHES = 0;

//  PURPOSE:  To tell the maximum total number of sandwiches to make.
const int MAX_NUM_SANDWICHES = 100;

//----		The Sandwich data-structure and related C functions:	----//

//  THIS WILL BE A *C* STRUCT!
struct Sandwich
{
  int bread_;
  int mainTopping_;
  int secondTopping_;
  Sandwich *nextPtr_;
};

//  PURPOSE:  To print the details of sandwich '*sandwichPtr'.  No return value.
void print(const Sandwich *sandwichPtr)
{
  printf("%s and %s on %s",
         MAIN_TOPPING_ARRAY[sandwichPtr->mainTopping_],
         SECOND_TOPPING_ARRAY[sandwichPtr->secondTopping_],
         BREAD_NAME_ARRAY[sandwichPtr->bread_]);
}

//  PURPOSE:  To return the address of a randomly-generated Sandwich order.
//	NOTE: The heap memory was obtained by 'malloc()', so get rid of it
//	with 'free()'.  No parameters.
Sandwich *makeOrder()
{
  Sandwich *toReturn = (Sandwich *)malloc(sizeof(Sandwich));

  toReturn->bread_ = rand() % NUM_BREADS;
  toReturn->mainTopping_ = rand() % NUM_MAIN_TOPPINGS;
  toReturn->secondTopping_ = rand() % NUM_SECOND_TOPPINGS;
  toReturn->nextPtr_ = NULL;
  return (toReturn);
}

//----		Initialization and usage-showing related C functions:	----//

//  PURPOSE:  To send to 'stderr' a message telling how to use this program.
//	No parameters.  No return value.
void showUsage()
{
  fprintf(stderr,
          "Usage:\tsandwichMaker <bufferLen> <num"
          "Makers> <numOrderers> <numSandwiches>\n"
          "Where:\n"
          "  <bufferLen>: length of the order buffer:\t%d <= N <= %d\n"
          "  <numMakers>: how many sandwich makers:\t%d <= N <= %d\n"
          "  <numOrderers>: how many sandwich orderers:\t%d <= N <= %d\n"
          "  <numOrderers>: total number of sandwiches:\t%d <= N <= %d\n",
          MIN_BUFFER_LEN, MAX_BUFFER_LEN,
          MIN_NUM_MAKERS, MAX_NUM_MAKERS,
          MIN_NUM_ORDERERS, MAX_NUM_ORDERERS,
          MIN_NUM_SANDWICHES, MAX_NUM_SANDWICHES);
}

//  PURPOSE:  To initialize '*bufferLenPtr' to be the length of the order
//	buffer, '*numMakersPtr' to be the number of makers, '*numOrderersPtr'
//	to be the number of orderers, and '*numSandwichesPtr' to be the
//	total number of sandwiches to process.  Values are obtained from the
//	command line arguments in 'argv[]', of which there are 'argc' of.
//	Returns '1' if initialization successful or '0' otherwise.
//
//	NOTE TO C++ CODERS:
//	In C++ this is more elegantly done with 'int&' instead of 'int*'.
//	We use 'int*' anyway, to give EVERYBODY more practice with pointers.
//
int didSuccessfullyInitializeArguments(int argc,
                                       char *argv[],
                                       int *bufferLenPtr,
                                       int *numMakersPtr,
                                       int *numOrderersPtr,
                                       int *numSandwichesPtr)
{
  //  I.  Application validity check:

  //  II.  Attempt to initialize:
  //  II.A.  Check validity of 'argc':
  if (argc < NUM_NECESSARY_CMD_LINE_ARGS)
  {
    return (0);
  }

  //  II.B.  Obtain arguments:
  // YOUR CODE HERE TO:
  // (1) Initialize the integer pointed to by bufferLenPtr to the integer whose text is in BUFFER_LEN_CMD_LINE_INDEX
  *bufferLenPtr = (int)strtol(argv[BUFFER_LEN_CMD_LINE_INDEX], NULL, 10);
  // (2) Initialize the integer pointed to by numMakersPtr to the integer whose text is in NUM_MAKERS_CMD_LINE_INDEX
  *numMakersPtr = (int)strtol(argv[NUM_MAKERS_CMD_LINE_INDEX], NULL, 10);
  // (3) Initialize the integer pointed to by numOrderersPtr to the integer whose text is in NUM_ORDERERS_CMD_LINE_INDEX
  *numOrderersPtr = (int)strtol(argv[NUM_ORDERERS_CMD_LINE_INDEX], NULL, 10);
  // (4) Initialize the integer pointed to by numSandwichesPtr to the integer whose text is in NUM_SANDWICHES_CMD_LINE_INDEX
  *numSandwichesPtr = (int)strtol(argv[NUM_SANDWICHES_CMD_LINE_INDEX], NULL, 10);

  //  II.C.  Check arguments:
  // YOUR CODE HERE TO return 0 if the corresponding integers are not in their proper ranges (look at the constant names):

  if (
      ((*bufferLenPtr > MAX_BUFFER_LEN) | (*bufferLenPtr < MIN_BUFFER_LEN)) |
      ((*numMakersPtr > MAX_NUM_MAKERS) | (*numMakersPtr < MIN_NUM_MAKERS)) |
      ((*numOrderersPtr > MAX_NUM_ORDERERS) | (*bufferLenPtr < MIN_NUM_ORDERERS)) |
      ((*numSandwichesPtr > MAX_NUM_SANDWICHES) | (*numSandwichesPtr < MIN_NUM_SANDWICHES)))
  {
    return (0);
  }

  //  III.  Finished:
  return (1);
}

//----		  Order list related class(es) and variable(s):		----//

//  PURPOSE:  To force whatever thread that runs this fnc to 'sleep()' for a
//  	  random amount of time.  No parameters.  No return value.
void hangOut()
{
  sleep(1 + rand() % 2);
}

//  THIS WILL BE A *C++* CLASS!
class OrderList
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the maximum length of the list:
  int maxLength_;

  //  PURPOSE:  To hold the length of the list:
  int length_;

  //  PURPOSE:  To point to the beginning of the list:
  Sandwich *startPtr_;

  //  PURPOSE:  To point to the end of the list:
  Sandwich *endPtr_;

  //  YOUR CODE HERE
  //  Perhaps add some more member vars
  pthread_mutex_t lock;
  pthread_cond_t isFull;
  pthread_cond_t isEmpty;

  //  II.  Disallowed auto-generated methods:
  //  No copy constructor:
  OrderList(const OrderList &);

  //  No copy assignment op:
  OrderList &operator=(const OrderList &);

protected:
  //  III.  Protected methods:

public:
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To initialize '*this' to an empty list.  No parameters.
  //	No return value.
  OrderList()
  {
    maxLength_ = MIN_BUFFER_LEN;
    length_ = 0;
    startPtr_ = NULL;
    endPtr_ = NULL;
    //  YOUR CODE HERE
    //  Perhaps initialize your vars
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&isFull, NULL);
    pthread_cond_init(&isEmpty, NULL);
  }

  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No
  //	return value.
  ~OrderList()
  {
    //  YOUR CODE HERE
    //  Perhaps destroy your vars
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&isFull);
    pthread_cond_destroy(&isEmpty);
  }

  //  V.  Accessors:
  //  PURPOSE:  To return the maximum length of the list.  No parameters.
  int getMaxLength()
      const
  {
    return (maxLength_);
  }

  //  PURPOSE:  To return the length of the list.  No parameters.
  int getLength()
      const
  {
    return (length_);
  }

  //  VI.  Mutators:
  //  PURPOSE:  To set the maximum length of '*this' list to 'newMaxLen'.
  //	No return value.
  void setMaxLength(int newMaxLen)
  {
    if (newMaxLen > 0)
    {
      maxLength_ = newMaxLen;
    }
  }

  //  VII.  Methods that do main and misc. work of class:
  //  PURPOSE:  To print the orders in '*this' list.  No parameters.  No
  //  	return value.
  void printList()
      const
  {
    const Sandwich *run;

    for (run = startPtr_; run != NULL; run = run->nextPtr_)
    {
      putchar(' ');
      putchar(' ');
      print(run);
      putchar('\n');
    }
  }

  //  PURPOSE:  To note that customer number 'custormerNum' has submitted an
  //	order for the Sandwich whose description is pointed to by
  //	'*sandwichPtr'.  No return value.
  void submitOrder(int customerNum,
                   Sandwich *sandwichPtr)
  {
    //  First critical section:
    //  Get lock and wait while the buffer is full

    pthread_mutex_lock(&lock);

    //  (getLength() is greater than or equal to getMaxLength())
    //  If the buffer full, then print

    //pthread condition wait

    //  just before you wait.

    while (getLength() >= getMaxLength())
    {
      printf("Orderer %d: \"Hurry up! I'm hungry!\"\n",
             customerNum);
      pthread_cond_wait(&isFull, &lock);
    }

    //  Print:
    printf("Orderer %d: \"I can *finally* order!\"\n",
           customerNum);
    //  just after you finish waiting.

    //  Just before it is safe to submit your order, print:

    printf("Orderer %d: \"I would like ", customerNum);
    print(sandwichPtr);
    printf(", please.\"\n");

    //  now actually change 'startPtr_', 'endPtr_' and 'length_'
    //  to actually place 'sandwichPtr' at the end of the list

    startPtr_ = sandwichPtr;
    endPtr_ = sandwichPtr + length_;
    length_ = --length_;

    printf("Orderer %d: \"The list has length %d:\"\n",
           customerNum, getLength());
    printList();
    //  End first critical section.

    hangOut(); //  Not in critical section

    //  Second critical section:
    printf("Orderer %d: \"Paying for ", customerNum);
    print(sandwichPtr);
    printf(".\"\n");
    //  Notify any maker thread that there is one more order
    //  End second critical section.
    pthread_cond_signal(&isEmpty);
    pthread_mutex_unlock(&lock);
  }

  //  PURPOSE:  To give sandwich maker number 'makerNum' the address of a
  //	sandwich to make.  Returns address of Sandwich.
  Sandwich *getOrder(int makerNum)
  {
    Sandwich *sandwichPtr;

    while (getLength() <= 0)
    {
      printf("Maker %d: \"Next!\"\n", makerNum);
      pthread_cond_wait(&isFull, &lock);
    }

    //  First critical section:
    //  Get lock and wait while the buffer is full
    //  (getLength() is less than or equal to 0)
    //  If the buffer empty, then print

    //  just before you wait.
    //  Print:
    printf("Maker %d: \"I *finally* have an order!\"\n",
           makerNum);
    //  just after you finish waiting.

    //  Just before it is safe to get an order, print:
    printf("Maker %d: \"Making ", makerNum);
    print(sandwichPtr);
    printf(".\"\n");
    startPtr_ = sandwichPtr;
    endPtr_ = sandwichPtr + length_;
    length_ = --length_;

    //  now actually change 'startPtr_', 'endPtr_' and 'length_'
    //  to actually make 'sandwichPtr' get the Sandwich at the beginning of the list
    printf("Maker %d: \"The list has length %d:\"\n",
           makerNum, getLength());
    printList();

    pthread_mutex_unlock(&lock);
    //  End first critical section.

    hangOut(); //  Not in critical section
    hangOut(); //  Not in critical section

    //  Second critical section:

    pthread_mutex_lock(&lock);
    printf("Maker %d: \"Finished ", makerNum);
    print(sandwichPtr);
    printf(".\"\n");
    //  Notify any orderer thread that there is space in the order list
    //  End second critical section.
    pthread_cond_signal(&isEmpty);
    pthread_mutex_unlock(&lock);
    return (sandwichPtr);
  }
};

//  PURPOSE:  To serve as the global OrderList instance used by all sandwich
//	orderers and makers.
OrderList orderList;

//----		     related class(es) and methods:		----//

//  PURPOSE:  To represent either a sandwich maker or a sandwich orderer, and
//	how many sandwiches they are to make or order.
struct Player
{
  int playerNum_;
  int numSandwiches_;
};

//  PURPOSE:  To be the function that sandwich maker threads run.  'vPtr'
//	points to a 'Player' instance describing the sandwich maker.  Returns
//	'NULL'.
void *makerFnc(void *vPtr)
{
  int i;
  Player *playerPtr = (Player *)vPtr;

  for (i = 0; i < playerPtr->numSandwiches_; i++)
  {
    //  Get a Sandwich* from global variable 'orderList' with getOrder.
    //  Then free() it
    hangOut();
  }

  return (NULL);
}

//  PURPOSE:  To be the function that sandwich orderer threads run.  'vPtr'
//	points to a 'Player' instance describing the sandwich maker.  Returns
//	'NULL'.
void *ordererFnc(void *vPtr)
{
  int i;
  Player *playerPtr = (Player *)vPtr;

  for (i = 0; i < playerPtr->numSandwiches_; i++)
  {
    //  Get a Sandwich* from makeOrder() above.
    //  Then submitOrder() it to global variable 'orderList' above
    hangOut();
  }

  return (NULL);
}

//  PURPOSE:  To run the sandwich orderer and making simulation.  'argv[]'
//	 has the array of arguments coming from the command line, and 'argc'
//	 tells how large 'argv[]' is.  Returns 'EXIT_SUCCESS' or success or
//	 'EXIT_FAILURE' otherwise.
int main(int argc,
         char *argv[])
{
  //  I.  Applicability validity check:
  int bufferLen;
  int numMakers;
  int numOrderers;
  int numSandwiches;

  if (!didSuccessfullyInitializeArguments(argc, argv,
                                          &bufferLen,
                                          &numMakers,
                                          &numOrderers,
                                          &numSandwiches))
  {
    showUsage();
    exit(EXIT_FAILURE);
  }

  //  II.  Run simulation:
  //  II.A.  Initialize data-structures:
  pthread_t *makerTidArray = (pthread_t *)malloc(sizeof(numMakers));
  pthread_t *ordererTidArray = (pthread_t *)malloc(sizeof(numOrderers));

  Player *makerArray = (Player *)malloc(sizeof(numMakers));
  Player *ordererArray = (Player *)malloc(sizeof(numOrderers));

  // Make makerTidArray point to an array of numMakers pthread_t* objects
  // Make ordererTidArray point to an array of numOrderers pthread_t* objects

  // Make makerArray point to an array of numMakers Player objects
  // Make ordererArray point to an array of numOrderers Player objects

  if (numMakers == 1)
  {
    makerArray[0].playerNum_ = 0;
    makerArray[0].numSandwiches_ = numSandwiches;
  }
  else
  {
    int approxNumSandwichesPerMaker = numSandwiches / numMakers;

    for (int i = 0; i < numMakers; i++)
    {
      makerArray[i].playerNum_ = i;
      makerArray[i].numSandwiches_ = approxNumSandwichesPerMaker;
    }

    makerArray[0].numSandwiches_ = numSandwiches -
                                   (approxNumSandwichesPerMaker * (numMakers - 1));
  }

  if (numOrderers == 1)
  {
    ordererArray[0].playerNum_ = 0;
    ordererArray[0].numSandwiches_ = numSandwiches;
  }
  else
  {
    int approxNumSandwichesPerOrderer = numSandwiches / numOrderers;

    for (int i = 0; i < numOrderers; i++)
    {
      ordererArray[i].playerNum_ = i;
      ordererArray[i].numSandwiches_ = approxNumSandwichesPerOrderer;
    }

    ordererArray[0].numSandwiches_ = numSandwiches -
                                     (approxNumSandwichesPerOrderer * (numOrderers - 1));
  }

  orderList.setMaxLength(bufferLen);

  //  II.B.  Initialize threads:
  //  YOUR CODE HERE:
  //  Launch all numMakers sandwich maker threads.
  //  Give them all the address of their corresponding makerArray[] object

  //  Launch all numOrderer sandwich orderer threads.
  //  Give them all the address of their corresponding ordererArray[] object

  for (int i = 0; i < numMakers; i++)
  {
    Player *newPlayer = new Player;
    pthread_create(&makerTidArray[i], NULL, makerFnc, newPlayer);
  }

  for (int i = 0; i < numOrderers; i++)
  {
    Player *newPlayer = new Player;
    pthread_create(&ordererTidArray[i], NULL, ordererFnc, newPlayer);
  }

  //  II.C.  Wait for threads:
  //  YOUR CODE HERE to wait for all threads:
  for (int i = 0; i < numMakers; i++)
  {
    pthread_join(makerTidArray[i], NULL);
  }

  for (int i = 0; i < numOrderers; i++)
  {
    pthread_join(ordererTidArray[i], NULL);
  }

  //  II.D.  Release resources:
  //  YOUR CODE HERE to release all heap memory:

  free(makerTidArray);
  free(ordererTidArray);

  free(makerArray);
  free(ordererArray);

  //  III.  Finished:
  return (EXIT_SUCCESS);
}