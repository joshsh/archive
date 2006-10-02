# Segfault before 100000
LIMIT=100000

for ((a=0; a <= LIMIT ; a++))
do
  echo "_new ns$a; _ns ns$a;"
done

echo "_size; _gc; _size;"
echo "_quit;"
