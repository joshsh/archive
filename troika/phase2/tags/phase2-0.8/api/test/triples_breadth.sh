LIMIT=100000

echo "s := 0;"

for ((a=0; a <= LIMIT ; a++))
do
  echo "^= s 42 42;"
done

echo "_size; _gc; _size;"
echo "_quit;"
