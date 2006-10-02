# NOTE: phase2 will handle a limit of 1,000,000 without error (and without
# aborting due to lack of memory), but you will become bored long before that.
LIMIT=1000

for ((a=0; a <= LIMIT ; a++))
do
  echo "int$a := 42;"
done

echo "_size; _gc; _size;"
echo "_quit;"
