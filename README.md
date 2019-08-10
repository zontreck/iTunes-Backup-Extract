# ZNI Extract v1
## Purpose

The purpose of this program and project is to make a easy to use command line tool, and perhaps eventually a GUI to extract data from a iTunes unencrypted backup for FREE.

With this tool you can easily extract your text messages.

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

# FAQ

## Why free?

Because there were no free alternatives, or even low cost.. to a bunch of paid software that literally just read a SQLite database and spit out your messages and such.. 
