#include "DG.h"

#include "Arduino.h"
#include "Consts.h"
#include "Global.h"

void DG_init() {
  pinMode(contactor_pin, OUTPUT);
  digitalWrite(contactor_pin, HIGH);

  pinMode(start_dg_pin, OUTPUT);
  digitalWrite(start_dg_pin, HIGH);

  pinMode(alarm_pin, OUTPUT);
  digitalWrite(alarm_pin, HIGH);
}

void start_dg_procedure(){
  start_dg();
  connect_dg(20);
}
void stop_dg_procedure(){
  disconnect_dg();
  stop_dg(20);
}

void start_dg() {
  digitalWrite(start_dg_pin, LOW);
}
void connect_dg(int after) {
  ts.add(DG1, after * 1000, [&](void*) {
    digitalWrite(contactor_pin, LOW);
    ts.remove(DG1);
  }, nullptr, false);
}


void disconnect_dg() {
  digitalWrite(contactor_pin, HIGH);
}
void stop_dg(int after) {
  ts.add(DG2, after * 1000, [&](void*) {
    digitalWrite(start_dg_pin, HIGH);
    ts.remove(DG2);
  }, nullptr, false);
}