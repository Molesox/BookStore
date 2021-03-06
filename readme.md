# Multithreaded BookStore

Welcome to the bookstore project that we created for the Operating Systems course at UniFR.
The general structure we chose tries to reflect the real world pretty closely :
There is a class for John, the librarian, another for the customer(s), and a main file to run
the program. Outside of those main files, there are utilities that help represent the books,
the shelves and the library itself, as well as another to contain the semaphore structure.

In our case, the library is a vector of shelves. Each shelf (representing a genre) is a 
hashmap (unordered_map). This structure helps to keep book searches and changes fast and
relatively efficient, which is not really necessary, but is always a good thing to have. 
It also helps to keep code changes simple and clean.

An important choice we made after designing the backbone for the project is to automatically
read in a huge amount of books at startup from a database found online. There are, at the 
time of writing, 16559 books in this database.

## Getting Started

To get started using our program, run the main executable with, as program parameter,
the path to the library csv file.

### Running the individual scenarios

Our program handles the starting of the scenarios. Simply run it, and it will ask you
which scenario you want to run.

### Data in the logs

The program, as soon as it is turned off, will generate 3 log files.
* One for the main method, containing some less interesting raw information
* One for John, containing information about his exchanges with the customers
* One for all the customers, indicating the books they took and the order in which things happened.

### Inner workings of the project

To mimic a real life situation, we chose to make John, the librarian, an independent
identity that has to access the library to be able to give out or take bake the books.
As such, There is a thread for John with a pointer to the library, and the customer
threads must go through John to get books or give them back. <br>The customers come into
the shop with a genre of interest, and an amount of books that they want to borrow.
John will happily give them the books, but has a set value of how many books he can give
out for a specific genre at a time. <br>This means that if the customer wants 3 books but 
John can only give him 2 at a time, the customer will take the first two, read them, give
them back, and then get the third one.

## Built With

* [CLion](https://www.jetbrains.com/clion/) - The IDE used for the base code
* [Qt](https://www.qt.io/download) - Used to make the GUI

## Git

We used [GitHub](https://github.com/Molesox/BookStore) to keep the project and handle 
merging. The project is also on the [DIUF GitLab](https://diuf-gitlab.unifr.ch/), 
but on is updated more often on GitHub.

## Authors

* **Daniel Sanz**
* **Bastian Stadelmann**
* **Maxence Feller**

