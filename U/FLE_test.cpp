/**
 * FLE_test.cpp
 *
 * Last edit: 6/6/04
 *
 * This is a test routine for the new "fuzzy logic" classes:
 *   {fuzzy_set, fuzzy_set_family, fuzzy_output_family, FLE}
 * as well as the non-GUI portion of my still unnamed Big Project, which
 * now includes the Fuzzy Logic Engine as a module.

Below we are working off of these input and output families:

	temp : cool 60, 65 warm 70, 80 hot;
	volume : small 100, 200 medium 200, 300 large;
	time : morning 5, 7 day 17, 20 evening;

	power : off, low, med, high;
	mood : contrary, obsequious, apathetic;

File output is to "fuzztest.U" and "fuzztest2.U"

*/

#include "U.h"



int main() {

  ifstream in;
  ofstream out;

  universe *u = new universe("FLE_test.cpp");
  FLE *heater = new FLE(u, "heater");

  fuzzy_set_family *temp = new fuzzy_set_family(u, "temp");
  temp->add("cool", 60, 60);
  temp->add("warm", 65, 70);
  temp->add("hot", 80, 80);
  fuzzy_set_family *volume = new fuzzy_set_family(u, "volume");
  volume->add("small", 100, 100);
  volume->add("medium", 200, 200);
  volume->add("large", 300, 300);
  fuzzy_set_family *time = new fuzzy_set_family(u, "time");
  time->add("morning", 5, 5);
  time->add("day", 7, 17);
  time->add("evening", 20, 20);

  fuzzy_output_family *power = new fuzzy_output_family(u, "power");
  power->add("off");
  power->add("low");
  power->add("med");   //! ###
  power->add("high");
  fuzzy_output_family *mood = new fuzzy_output_family(u, "mood");
  mood->add("contrary");
  mood->add("obsequious");
  mood->add("apathetic");

  heater->add(temp);
  heater->add(volume);
  heater->add(time);
  heater->add(power);
  heater->add(mood);

  out.open("fuzztest.U");
  u->serialize(out);
  out.close();

  delete u;

  in.open("fuzztest.U");
  u = new universe(in);
  in.close();

  out.open("fuzztest2.U");
  u->serialize(out);
  out.close();

}
