Tests=Tests/KMP/*
for test in $Tests
do 
    echo "\nStart test"
    echo "Input:"
    cat $test
    ./KMP < $test
done