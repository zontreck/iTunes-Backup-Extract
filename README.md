# ZNI Extract v1
## Purpose

The purpose of this program and project is to make a easy to use command line tool, and perhaps eventually a GUI to extract data from a iTunes unencrypted backup for FREE.

With this tool you can easily extract your text messages.

## Getting Started

As you will see when you launch the program, you need a file called "3d0d7e5fb2ce288813306e4d4636395e047a3d28" out of your iOS backup.
And while it does prompt you to find the 5a file as well, it doesnt actually do anything to that file aside from open it with sqlite. 
I will be adding in the call log support eventually as well as HTML but for now i was happy i even got the text messages to work so effectively.

## Build Instructions

Presently this tool is only tested in a linux environment!

```bash
sudo apt-get install build-essential libsqlite3-dev sqlite3
cd <project_folder>
gcc -c main.cpp -o main.o
g++ main.o -o extract_tool -lsqlite3
```

## Contributing

If you find a bug you can either send in a pull request or open a issue

Same goes for features and such, just open a PR or issue, it'll get looked at.

## Author
Tara Piccari created this software on 8/10/2019 and this software is licensed under the GPLv2

# FAQ

## Why free?

Because there were no free alternatives, or even low cost.. to a bunch of paid software that literally just read a SQLite database and spit out your messages and such.. 
