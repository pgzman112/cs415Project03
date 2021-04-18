// Preston Zimmerman, Diana Arce
// 4/17/21
// Project 3 Trie tree and Ternary Search Tree

Directions: This one should be real straight forward.

    1) unzip file to local hard drive that you can easily navigate your command terminal to.
    2) type in 'make'
    3) thats it, now feel free to run program with './P3.out alpha_words.txt 1' or which ever combination you feel like
            - If you enter a text file that isnt there or anything besides 1/2 for the 2nd arg it will exit and prompt for diff input.
    4) give students an A so they don't have to take the final :)
    5) if this doesn't work on your local machine for any reason I also put it up on Blue and ran it and it works there.
    so just upload the entire file using sftp or filezilla and run on blue if any errors exist.

    Note:
        - I noticed something interesting when running alpha_words in mode 1. when searching abac the first time it
          takes quite a long time, if you search it a second time it will find the correct strings in about half the time roughly
          (this was on my machine, I dont know how it will work on others). The first time it is slightly slower than the TST, I think
          this is due to the fact that perhaps abac is very easy to locate in the tst just in the way that its structured. But whats
          interesting is that on the second search of abac the Trie search is cut in half and is slower than the TST for all subsequent searches
          This seems to be isolated to the string abac in the case that Trie is slower the first time than TST. But on most strings
          the first search in the Trie structure is slower than subsequent searches. I am wondering if this has something to do with
          compiler laziness when setting up all of the arrays of null pointers? Or it is possible that the way I am timing things
          is running into problems (since some of the search functions require recursion for TST I was worried that may possible mess
          up the autocomplete timers). This is only an issue in when searching individual strings and in certain cases.
          90% of the time running on my machine the Trie searches faster than the TST.
          And once you run user test mode 2 its quite obvious how much faster the Trie is to search than the TST.

        - Also on some of the really small text files it might say that the TST takes up zero bytes. I think this
        is due to the way in which I am getting memory usage and it is just to small that it comes out to zero.
        I have them stored as long long ints I believe so it is possible that when it does the subtraction it is a
        fraction and then truncates to 0. I ended up leaving this because well its really tiny it may as well be zero. That
        would never happen for any input text of halfway decent size though.