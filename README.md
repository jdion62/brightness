# brightness
A program that represents images internally and will brighten them

Made in collaboration with John Bertsch for a school project

Group: John Bertsch and Jacob Dion


Additional Resources:

Design talks with other classmates

Input questions on ta411@cs.uri.edu


Completed Tasks:

Brightness was completed and should work as expected.

Fgroups was written and should function as appropriate:

	Notes on fgroups:

	Our implementation of fgroups uses some defined constants as an

	arbitrary upper bound on the size of the name field and should be easily

	scalable by changing this value to suit systems requiring larger strings


PART C"


Problems solved with fgroups:

1.) find

	find is a bas function that lends itself well to fgroups. It can solve

	this problem by listing it's parent directories as a fingerprint; For 

	example, in a simple structure where CSC411/ is the parent directory

	with a sub-drectory called test/ and files called prog.c, and prog.h,

	prog.c, prog.h, and test/ all have the same fingerprint but everything

	within test/ has a different fingerprint unless preceded by test/ in

	the name. (fingerprint is the same if the name is test/test_case.txt)


2.) Sorting Algorithm

	some sorting algorithms depend on smaller arrays. In these situations,

	it may be useful to have a working fgroup class. For example, sorting

	a list alphabetically by first seperating the list into groups

	determined by the first letter of the string, groups would likely be

	named in accordance to that letter (A, B, C,..., X, Y, Z). From this

	point, you could make any modified version of a sort function.


3.) Lectures/Lab

	Taking a step away from computer science, fgroups could be useful for

	lecturers. If you have 2 lectures and 4 labs, you could build class

	rosters using fingerprints. Fingerprints could include SEC1 and SEC2

	along with LAB1 through LAB4.


Time Estimate: 

15 hours of actual code writing (a solid 4 of which were spent trying to 

figure out how to free atoms).
