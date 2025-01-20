# Rules of the Exam

## Due Date & Rules
The deadline for submission is Friday, Sept 1 at 8 PM Eastern. 

Although you can work on this assignment whenever you like, I would prefer that you treat this work like a normal exam: try to sit down and complete it in a couple of hours, **timing yourself to see how long it takes you.** You can take breaks as you need, but it will be important for you to report how long you spend working on this problem.

You can use internet resources as you like, but do not Google this particular problem. 

## Academic Integrity
This exam is not proctored. This is a chance for you to honestly show the course staff *and yourself* what your level of mastery is. If I do detect evidence of cheating or plagiarism (like copying the work from someone else who has done this problem already), there will be serious repercussions. 

# Content of the Exam
You will implement some of the features found in social networking sites like Instagram, Twitter, or TikTok. 

## 1. Parsing Data
You will write code that reads a file with a specific format.

Each line of the file will start with a person's name, followed by a colon (`:`), followed by a comma (`,`) separated list of the names of the other people this person follows. For example, the file might look like this:

```
Harry: Adrian, Phillipp, Dana
Dana: Vivian, Jintong
Jintong: Dana
```

The above is the content of the file `example.txt`, provided to you. You can assume that:
- the file is correctly formatted
- names do not contain spaces
- following is not necessarily a reciprocal relationship (e.g. Harry follows Dana but Dana does not follow Harry)

## 2. Analyzing the Network

You will write two methods in the class `SocialNetworkRunner` (a stub has been provided for you). You must include a **`main` method** in `SocialNetworkRunner.java` so that we can run your program, but you can leave the body of the method blank if you wish.

---
### A. `public String mostFollowers()`
This method will return the name of the person with the most followers; *that is, the person who is followed by the largest number of other users*. This method should return a `String` and take no arguments.

### B. `public int followersInCommon(String personOne, String personTwo)`
This method will take two `Strings` as parameters, both representing the name of a person in the network. The method should return an `int` representing how many other people they follow in common. For example, Harry and Jintong follow Dana in common.

---

You can create as many additional classes and methods as you feel appropriate. You may also modify the privacy or `static` aspect of either of these methods, although you should address your choice in the `WRITEUP.txt` file you will submit. More details on `WRITEUP.txt` below.

## 3. Design
Your program should follow good software design principles. Please include a 2-3 sentence explanation in `WRITEUP.txt` of which design choices you made and why.

## 4. Testing
You need to include some unit testing using JUnit for the program that you write. You must write at least 6 unit tests and include these tests in your submission. Include in `WRITEUP.txt` a brief 2-3 sentence explanation of why you wrote the tests that you did.

## 5. GitHub
This assignment writeup was originally published on GitHub in a repository that has been created for you. You are required to clone this repository to your local computer, make your changes there, and then commit and push the results back into this remote repository. Please include in `WRITEUP.txt` a list of the commands that you use to clone the repo, commit changes, and push them. 

## 6. Timing
Do not forget to include how long you spent on this assignment in `WRITEUP.txt`!

# Submission
As above, you will submit your work including all `.java` files, all test cases, and `WRITEUP.txt` with all of the required information to the GitHub repo. Your work will be graded from there.

