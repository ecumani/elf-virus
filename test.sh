
gcc virus.c -o virus

filesize=`ls -l virus|awk '$5 {print $5}'`
progsize=`awk '/define SIZE/ {print $3}' virus.c `

if [ $filesize -eq $progsize ];
then
    echo "file sizes are correct"
    
    echo ""

    echo "running virus"
    ./virus
    
    echo ""

    echo "copying echo to this directory"
    cp /bin/echo .

    echo ""

    echo "size of echo"
    du -b  echo

    echo ""

    echo "running virus"
    ./virus

    echo ""

    echo "size of the echo now"
    du -b echo

    echo ""

    echo "running './echo "I am healthy!"'"
    ./echo "I am healthy!"

    echo ""

    echo "copying ls"
    cp /bin/ls .

    echo ""

    echo "we run echo which will infect ls"
    ./echo

    echo ""

    echo "running ls (nothing to infect)"
    ./ls

    echo ""

    echo "Copying cat and grep"
    cp /bin/cat .
    cp /bin/grep .
    
    echo ""
    
    echo "changing file permisions for grep"
    chmod u-w-r grep
    
    echo ""

    echo "running ls will infect cat"
    ./ls

    echo ""

    echo "createing temp.txt"
    touch temp.txt
    echo "this is 1st line" >> temp.txt
    echo "this is 2nd line" >> temp.txt

    echo ""

    echo "running './cat temp.txt -n"
    ./cat temp.txt -n

    echo ""

    echo "running './grep "this" temp.txt' "
    echo "hopefully virus does not infect grep"
    grep "this" temp.txt

    echo ""

    echo "compiling sam.c"
    gcc sam.c

    echo ""
    
    echo "size of a.out"
    du -b a.out

    echo ""

    echo "executing a.out (not infected yet)"
    ./a.out

    echo ""

    echo "excuting grep"
    grep "this" temp.txt

    echo ""

    echo "size of a.out(still not infected)"
    du -b a.out

    echo ""

    echo "executing echo"
    ./echo

    echo "size of a.out(infected)"
    du -b a.out

    echo ""

    echo "executing a.out"
    ./a.out

    echo ""

    echo "removing files"
    rm -f echo ls a.out grep cat temp.txt virus


else
    echo "file size do not match"
    
fi
