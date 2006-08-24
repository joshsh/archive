a b c a d

A single "next" may be used if no two lists share a node.
Explicit "nodes" may be omitted if the list contains no repeat values.

0 (0 last (next (1 next ^)) nix) (last (1 last ^)) = <+>.

################################################################################

tense ([0 (1 (2 (express_verb 3)))] (apply_to_all modify)).


0 (express (0 state println)) println
  ((0 root (0 tense (1 (express verb)))) +).


_statement express.
  
"-- Modify the statement --",
_subject (number (plural ^)),
_object (Continental apply),
_statement express.

"-- Modify it a little more --",
_verb (tense (past perfect ^)),
_verb (inexplicably apply),
_statement (subject (_object ^)),
_statement (object (_subject ^)),
_statement express.


println (apply_to_all modify) = print_all.
one newnode = first.
first (two append) next (three append).











"-- Build an English statement (sentence) --",
cat (the apply) (fat apply) = _subject,
sit (past_imperfect apply) (tense (past imperfect ^)) = _verb,
mat (the apply) (small apply) = _object,
_statement
  (new statement)
  (subject (_subject ^))
  (verb (_verb ^))
  (object (_object ^)),
_statement express.


  
cat
  (new noun)
  (plural regular).
mat
  (new noun)
  (plural regular).
fat
  (new adjective)
  ("fatter" ("fattest" (comparative irregular))).
sit
  (new verb intransitive)
  ("sat" ("sat" (tense irregular))). 
  
#################################################################################

$ (#1001 set_of) (story title) println.

blind mouse   -->   "Three Blind Mice"
angry man     -->   twelve angry men
Arabian night -->   one thousand and one Arabian nights
              -->   "One Thousand and One Arabian Nights"

              -->   _One_Thousand_and_One_Arabian_Nights_

0 (@ 1) = title.
title (story (quotate ^)).
title (song (quotate ^)).
title (book (underline ^)).

int_lessthan = <. int_greaterthan = >.

"-- Bind selected integer values to their English names --".
0 intmap (English (1 ^)) = x.
#0 (zero x). #1 (one x). #2 (two x). #3 (three x). #4 (four x). #5 (five x).
#6 (six x). #7 (seven x). #8 (eight x). #9 (nine x). #10 (ten x).
#11 (eleven x). #12 (twelve x). #13 (thirteen x). #14 (fourteen x).
#15 (fifteen x). #16 (sixteen x). #17 (seventeen x). #18 (eighteen x).
#19 (nineteen x). #20 (twenty x). #30 (thirty x). #40 (forty x).
#50 (fifty x). #60 (sixty x). #70 (seventy x). #80 (eighty x). #90 (ninety x).
#100 (hundred x). #1000 (thousand x). #1000000 (million x).
"x" forget.



0 next (1 (0 value 2 append) next @).
0 next (0 value 1 newnode (1 [1 (0 (1 (2 $)))])).
apply_to_all (generate ($ ^)).



"-" = HYPHEN. " " = SPACE. "" = NIL.
"_" = UNDERSCORE. "\"" = QUOTE.
1 (0 +) (1 +) = enclose.
0 (2 +) (1 +) = infix.
HYPHEN infix = hyphenate.
0 (1 value (2 infix)) (1 next @).
infix (list ($ ^)).
NIL (0 (SPACE (infix list))) (QUOTE enclose) = quotate.
NIL (0 (UNDERSCORE (infix list))) (UNDERSCORE enclose) = underline.

breakfast:
  (new: noun)
  (plural: noun regular).
I:
  (new: pronoun)
  ("I": "me": "my": (case: pronoun singular)).
  ("we": "us": "our": (case: pronoun singular)).
  

  


case_X_number (new list).
1 (SPACE +) (0 +) (2 enqueue).
number (0 (case_X_number $) (apply_to_all modify)).
case ($ (apply_to_all modify)).