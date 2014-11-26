---
layout: article
title: ""
date: null
modified: null
published: true
toc: false
---

##Exploiting Buffer Overflows in C

This problem will give you some hands-on experience to understand buffer overflows and how to exploit them. You will carry out the project using a virtual machine, on your own computer. In carrying it out, you will have to answer specific questions, given at the bottom, to show that you have followed each of the necessary steps.

###Installing and running the Virtual Machine

This problem will use a virtual machine in the [VirtualBox](https://www.virtualbox.org/) format. The overall VirtualBox manual is available [here](https://www.virtualbox.org/manual/UserManual.html).
<br />

The first step is to install VirtualBox on your computer. There are specific instructions for doing so for computers running [Windows](https://www.virtualbox.org/manual/ch02.html#installation_windows), [Linux](https://www.virtualbox.org/manual/ch02.html#install-linux-host), and [Mac OSX](https://www.virtualbox.org/manual/ch02.html#idp52693904).
<br />

The next step is to download the virtual machine image, in OVF format, that we will use for the project. It has extension <code>.ova</code> meaning it is an <em>archive</em> with all of the relevant materials in it. The file can be found [here](https://d28rh4a8wq0iu5.cloudfront.net/softwaresec/virtual_machine/mooc-vm.ova?response-content-type=application%2Foctet-stream&amp;a=1&amp;response-content-disposition=attachment). This virtual machine runs a version of [Ubuntu Linux](http://www.ubuntu.com/).
<br />

Finally, you must import this OVF file, which is called <code>mooc-vm.ova</code>, and run it. To import it, it should be as simple as double-clicking the <code>.ova</code> file. Doing so will start VirtualBox and ask you whether to import it the image. You should then click &#8220;import&#8221;. Alternatively, rather than double clicking the archive file, you can select &#8220;File&#8221; -&gt; &#8220;Import appliance&#8221; from the Manager window and select the file. Further instructions are available [here](https://www.virtualbox.org/manual/ch01.html#ovf).
<br />

Having imported the VM, you should see it in your list of VMs. Select it and click &#8220;Start&#8221;. This will open a window running the virtual machine, starting up Ubuntu Linux. When you get to a login screen, use username &#8220;seed&#8221; and password is &#8220;dees&#8221; (but without quotes). Then start up a terminal window &#8211; there is an icon in the menu bar at the top for doing so (it looks like a computer monitor).

###The vulnerable program

We have placed a C program <code>wisdom-alt.c</code> in the <code>projects/1</code> directory in the virtual machine. Type <code>cd projects/1</code> to change into this directory. If you type <code>ls</code> you will see that also in this directory is a compiled version of the program, called <code>wisdom-alt</code>. This executable was produced by invoking <code>gcc -fno-stack-protector -ggdb -m32 wisdom-alt.c -o wisdom-alt</code> (in case you accidentally delete it and need to reproduce it).

####Running the program

The program reads data from the <code>stdin</code> (i.e., the keyboard) and writes to <code>stdout</code> (i.e., the terminal). You can run the program by typing <code>./wisdom-alt</code> on the command prompt. When we do this, we see the following greeting:

<pre><code>seed@seed-desktop:~/projects/1$ ./wisdom-alt
Hello there
1. Receive wisdom
2. Add wisdom
Selection &gt;
</code></pre>

At this point, it is waiting for the user to type something in. Typing the number 1 allows you to &#8220;receive wisdom&#8221; and typing 2 allows you to &#8220;add wisdom&#8221;. Extending the interaction, suppose we type 1 (and a carriage return).

<pre><code>seed@seed-desktop:~/projects/1$ ./wisdom-alt
Hello there
1. Receive wisdom
2. Add wisdom
Selection &gt;1
no wisdom
Hello there
1. Receive wisdom
2. Add wisdom
Selection &gt;
</code></pre>

Notice that it outputs no wisdom and then repeats the greeting. Now if we type 2 we can try to add some wisdom; here's what happens:

<pre><code>Selection &gt;2
Enter some wisdom
</code></pre>

Now the program is waiting for the user to type something in. Suppose we type in <code>sleep is important</code> and press return. Then we will get the standard greeting again. If we type 1 at that point we will get the following:

<pre><code>Selection &gt;2
Enter some wisdom
sleep is important 
Hello there
1. Receive wisdom
2. Add wisdom
Selection &gt;1
sleep is important
Hello there
1. Receive wisdom
2. Add wisdom
Selection &gt;
</code></pre>

We can continue to add wisdom, by typing 2. For example, if we did this sequence again, with the entry <code>exercise is useful</code>, we would get:

<pre><code>Selection &gt;2
Enter some wisdom
exercise is useful
Hello there
1. Receive wisdom
2. Add wisdom
Selection &gt;1
sleep is important
exercise is useful
Hello there
1. Receive wisdom
2. Add wisdom
Selection &gt;
</code></pre>

We can keep doing this as long as we like. We can terminate interacting with the program by typing control-D.

####Crash!

This program is vulnerable to a buffer overflow. It is easy to see there is a problem, by typing in something other than 1 or 2. For example, type in 156.

<pre><code>seed@seed-desktop:~/projects/1$ ./wisdom-alt
Hello there
1. Receive wisdom
2. Add wisdom
Selection &gt;156

Segmentation fault
</code></pre>

In fact, the program has at least <em>two</em> vulnerabilities; the above is demonstrating one of them, but there is one other. Your job in this lab is to find and exploit both vulnerabilities. The lab will guide you through steps to do so, and you will answer questions as you go along.

###Exploiting the program

We are now going to show you some tools you'll need to exploit this program. 

####Entering binary data

To exploit the program, you will need to enter non-printable characters, i.e., <em>binary data</em>. To input binary data to the program, use the following command line instead:

<pre><code>./runbin.sh</code></pre>

Then we can type in binary-format strings (e.g., with hex escaping). For example: 

<pre><code>seed@seed-desktop:~/projects/1$ ./runbin.sh
Hello there
1. Receive wisdom
2. Add wisdom
Selection &gt;2
Enter some wisdom
\x41\x41
Hello there
1. Receive wisdom
2. Add wisdom
Selection &gt;1
AA
</code></pre>

In the above, <code>\x41\x41</code> represents two bytes, defined in hexadecial format. 41 in hex is 65 in decimal, which in ASCII is the character <code>A</code>. As a result, when we ask for wisdom, the program prints <code>AA</code>. Entering something like <code>\x07</code> would be a byte 7. This is not a printable character, but is the &#8220;bell&#8221;. So when it &#8220;prints,&#8221; you would actually hear a sound (if sound were enabled on this VM).
<br />

To exploit the program, you will have to enter sequences of binary bytes that contain addresses, which are 4-byte (i.e., 32-bit) words on the VM. The x86 architecture is &#8220;little-endian&#8221;, meaning that the bytes in a word are stored from least significant to most significant. That means that the hexadecimal address 0xabcdef00 would be entered as individual bytes in reverse order, i.e., \x00\xef\xbc\xab. Here is a [refresher on endianness](http://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Data/endian.html), if you need it.
<br />

<em>Note</em>: <code>runbin.sh</code> is a shell script that is just a wrapper around the following code:

<pre><code>while read -r line; do echo -e $line; done | ./wisdom-alt</code></pre>

This is what is converting the hex digits into binary before passing them to the <code>wisdom-alt</code> program. When carrying out the lab, please use the <code>runbin.sh</code> program, and not the above code directly, or your answers may be slightly off, as discussed at the end.

####Using GDB

To exploit the program, you will have to learn some information about how it is laid out in memory. You can find out this information using the <code>gdb</code> program debugger. You can <em>attach</em> <code>gdb</code> to your running program, and then use it to print information about the state of that program, and step through executions of that program. 
<br />

To attach <code>gdb</code> to <code>wisdom-alt</code>, you should first invoke the above command line, and then, in a separate terminal, from the <code>projects/1</code> directory invoke the following line: 

<pre><code>gdb -p `pgrep wisdom-alt`</code></pre>

Should you encounter any errors running the above line, you can first <code>pgrep wisdom-alt</code> to obtain the PID of <code>wisdom-alt</code>, and then run <code>gdb -p [PID]</code>.
<br />

The <code>-p</code> option to <code>gdb</code> tells it to attach to a running program with the PID given to the option. The command <code>pgrep wisdom-alt</code> searches the process table to find the PID of the <code>wisdom-alt</code> program; this PID is then fed as the argument to <code>-p</code>. <em>Be warned</em>: If you have multiple <code>wisdom-alt</code> programs running, you may not attach to the one you expect! Make sure they are all killed (perhaps by killing and restarting the terminals you started them in) if you run into trouble.
<br />

Once you have connected to the process, you can start using <code>gdb</code> commands to start examining its state and controlling it. For example: 

<pre><code>seed@seed-desktop:~/projects/1$ gdb -p `pgrep wisdom-alt`

GNU gdb 6.8-debian
Copyright (C) 2008 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later &lt;http://gnu.org/licenses/gpl.html&gt;
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type &quot;show copying&quot;
and &quot;show warranty&quot; for details.
This GDB was configured as &quot;i486-linux-gnu&quot;.
Attaching to process 29727
Reading symbols from /home/seed/projects/1/wisdom-alt...done.
Reading symbols from /lib/tls/i686/cmov/libc.so.6...done.
Loaded symbols for /lib/tls/i686/cmov/libc.so.6
Reading symbols from /lib/ld-linux.so.2...done.
Loaded symbols for /lib/ld-linux.so.2
0xb7fe1430 in __kernel_vsyscall ()
(gdb) 
</code></pre>

This shows starting <code>gdb</code> and attaching it to a running <code>wisdom-alt</code> process. Then the <code>gdb</code> command prompt comes up. At this point, the execution of that program is paused, and we can start entering commands. For example:

<pre><code>(gdb) break wisdom-alt.c:100
Breakpoint 1 at 0x80487ea: file wisdom-alt.c, line 100.
(gdb) cont
Continuing.
</code></pre>

Here we enter a command to set a breakpoint at line 100 of <code>wisdom-alt.c</code>. Then we enter command <code>cont</code> (which is short for <code>continue</code>) to tell the program to resume its execution. In the other terminal, running <code>wisdom-alt</code> we enter <code>2</code> and press return. This causes execution to reach line 100, so the breakpoint fires, and the <code>gdb</code> command prompt comes up again, pausing the program in the process. 

<pre><code>Breakpoint 1, main () at wisdom-alt.c:100
100            int s = atoi(buf);
(gdb) next
101            fptr tmp = ptrs[s];
(gdb) print s
$1 = 2
(gdb) print &amp;r
$2 = (int *) 0xbffff530
(gdb) cont
Continuing.
</code></pre>

Above, we control the program by stepping using &#8220;next&#8221;, which executes the current line of code, proceeding to the next. Then we print the contents of variable <code>s</code> with &#8220;print&#8221;, and it displays the value we entered in the other terminal. Then we print the address of the variable <code>r</code>. Finally, we continue execution by entering &#8220;cont&#8221;. In the other terminal we see the prompt to enter some wisdom.
<br />

When you are done working with gdb (perhaps when you've terminated the other program), just type <code>quit</code> to exit.
<br />

The basic GDB commands you will want to use are those we have already demonstrated: setting break points, stepping through execution, and printing values. If you are not familiar with GDB already, a quick GDB reference is available [here](http://www.cs.umd.edu/class/spring2014/cmsc414-0201/downloads/gdb-refcard.pdf), and a more in depth GDB tutorial is available [here](http://cs.brown.edu/courses/cs033/docs/guides/gdb.pdf"). You would find it helpful to be familiar with the &#8220;print&#8221;, &#8220;break&#8221;, and &#8220;step&#8221; commands. A full [GDB user's manual](http://www.gnu.org/software/gdb/documentation/) is also available.

##Questions

You are now ready to start your process of developing an exploit.
<br />

The first step is to identify where the buffer overflows are. To do that you will have to look through the code of <code>wisdom-alt.c</code>. You can do this by using an editor on Linux virtual machine, like <code>vi</code> or <code>emacs</code>, both of which are installed. Alternatively you can look through the file on your own machine outside of the VM, in an editor of your choice &#8212; the file is available [here](https://d28rh4a8wq0iu5.cloudfront.net/softwaresec/project1/wisdom-alt.c?response-content-type=application%2Foctet-stream&amp;a=1&amp;response-content-disposition=attachment).
<br />

After looking over the code to see how it works, answer the following four questions:

<ul>
<li>There is a stack-based overflow in the program. What is the name
of the stack-allocated variable that contains the overflowed buffer?</li>
<li>Consider the buffer you just identified: Running what line of code will overflow the buffer? (We want the line number, not the code itself.)</li>
<li>There is another overflow, <em>not dependent at all on the first</em>, of a <em>non</em>-stack-allocated
buffer. What variable contains this buffer?</li>
<li>Consider the buffer you just identified: Running what line of code overflows the buffer? (We want the number here, not the code itself.)</li>
</ul>

Now use GDB to examine the running the program and answer the following questions. These questions are basically going to walk you through constructing an exploit of the non-stack-based overflow vulnerability you just identified. We will do less &#8220;hand holding&#8221; when asking about exploiting the stack-allocated buffer.
<br />

Write your answers to the questions on your PDF submission for this homework.
<br />

<strong>Important</strong>: When carrying out the lab, you must follow the instructions given above for running the program (using <code>runbin.sh</code>) and using GDB (attaching to <code>wisdom-alt</code> in a separate terminal) <strong>exactly</strong> or else the answers you get may not match the ones we are expecting. In particular, the addresses of stack variables may be different. These addresses might also be different if you have altered any environment variables in the Ubuntu terminal. To confirm that things are as they should be, recall the GDB interaction above, where we print the address <code>&amp;r</code> with the result being <code>0xbffff530</code> &#8211; if you are not getting that result when you reproduce that interaction then something is wrong. You should restart fresh terminals and begin from scratch, following the instructions exactly.
<br />

On to the first exploit:

<ul>
<li>What is the address of <code>buf</code> (the local variable in the main function)? Enter the answer in either hexadecimal format (a <code>0</code>x followed by 8 &#8220;digits&#8221; 0&#8211;9 or a-f, like <code>0xbfff0014</code>) or decimal format. Note here that we want the address of <code>buf</code>, not its contents.</li>
<li>What is the address of <code>ptrs</code> (the global variable) ? As with the previous question, use hex or decimal format.</li>
<li>What is the address of <code>write_secret</code> (the function) ? Use hex or decimal.</li>
<li>What is the address of <code>p</code> (the local variable in the <code>main</code> function) ? Use hex, or decimal format.</li>
<li>What input do you provide to the program so that <code>ptrs[s]</code> reads (and then tries to execute) the contents of local variable <code>p</code> instead of a function pointer stored in the buffer pointed to by <code>ptrs</code>? You can determine the answer by performing a little arithmetic on the addresses you have already gathered above &#8211; be careful that you take into account the size of a pointer when doing pointer arithmetic. If successful, you will end up executing the <code>pat_on_back</code> function. Enter your answer as an (unsigned) integer.</li>
<li>What do you enter so that <code>ptrs[s]</code> reads (and then tries to execute) starting from the 65th byte in <code>buf</code>, i.e., the location at <code>buf[64]</code>? Enter your answer as an (unsigned) integer.</li>
<li>What do you replace <code>\xEE\xEE\xEE\xEE</code> with in the following input to the program (which due to the overflow will be filling in the 65th&#8211;68th bytes of <code>buf</code>) so that the <code>ptrs[s]</code> operation executes the <code>write_secret</code> function, thus dumping the secret? (Hint: Be sure to take endianness into account.) <code>771675175\x00AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\xEE\xEE\xEE\xEE</code></li>
</ul>

Now let's consider the other vulnerability:

<ul>
<li>Suppose you wanted to overflow the <code>wis</code> variable to perform a stack smashing attack. You could do this by entering 2 to call <code>get_wisdom</code>, and then enter enough bytes to overwrite the return address of that function, replacing it with the address of <code>write_secret</code>. How many bytes do you need to enter prior to the address of <code>write_secret</code>?</li>
</ul>

To work out the answer here, you might find it useful to use the GDB <code>backtrace</code> command, which prints out the current stack, and the <code>x</code> command, which prints a &#8220;hex dump&#8221; of the bytes at a given address. For example, by typing <code>x/48xw $esp</code> you would print out 48 words (the <code>w</code>) in hexadecimal format (the <code>x</code>) starting at the address stored in register <code>$esp</code>. 
