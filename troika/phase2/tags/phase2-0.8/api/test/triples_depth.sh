# Segfault before 100000
LIMIT=10000

echo "first := 0; cur := first;"
echo "next := 0;"

for ((a=0; a <= LIMIT ; a++))
do
  echo "last := cur;"
  echo "cur := 0;"
  echo "^= last next cur;"
done

echo "_size; _gc; _size;"
echo "_quit;"
