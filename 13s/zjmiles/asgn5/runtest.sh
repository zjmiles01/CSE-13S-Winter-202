# Usage:    bash runtest.sh FILENAME

# Check that one command-line argument is given.
if [ "$1" = "" ]
then
    echo 'Usage:  bash runtest.sh FILENAME'
else
    # Automatically determine if this laptop is running Apple silicon or x86.
    CPUTYPE=`lscpu | grep Architecture: | awk -e '{print $2;}'`

    # If the automation above doesn't work, then uncomment one of the lines
    # below to force the choice.
    #CPUTYPE=aarch64
    #CPUTYPE=x86

    # Set REF_COMPRESS and REF_DECOMPRESS to the paths of appropriate executable files.
    if [ $CPUTYPE = 'aarch64' ]
    then
        REF_COMPRESS='../../resources/asgn5/compress_arm'
        REF_DECOMPRESS='../../resources/asgn5/decompress_arm'
    else
        REF_COMPRESS='../../resources/asgn5/compress_x86'
        REF_DECOMPRESS='../../resources/asgn5/decompress_x86'
    fi

    # Confirm that the executable files are where we expect them to be.
    if ! [ -f $REF_COMPRESS ]
    then
        echo "Can't find reference program $REF_COMPRESS"
    else
        if ! [ -f $REF_DECOMPRESS ]
        then
            echo "Can't find reference program $REF_DECOMPRESS"
        else
            # Run the executables.
            $REF_COMPRESS $@ -o expected.comp
            ./compress $@ -o got.comp
            ./decompress expected.comp -o got.decomp

            # The compressed file is binary, and the decompressed file
            # might have bugs. So compare hexadecimal representations
            # of the files.
            xxd expected.comp > expected.comp.xxd
            xxd got.comp > got.comp.xxd
            xxd $@ > expected.decomp.xxd
            xxd got.decomp > got.decomp.xxd

            # Print any difference.
            # The diff -s option reports when the files are identical.
            echo Compression Result:
            diff -us expected.comp.xxd got.comp.xxd
            echo Decompression Result:
            diff -us expected.decomp.xxd got.decomp.xxd
        fi
    fi
fi

