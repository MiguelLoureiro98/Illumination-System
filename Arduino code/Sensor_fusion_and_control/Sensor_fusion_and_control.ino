#include <math.h>

const int lux_lim = 100;
const int sensor_1 = 0;
const int sensor_2 = 1;
const int sensor_3 = 2;
const int n_sensors = 3;
const int pin = 2;

unsigned long prev_time = 0;
const unsigned long sample_time = 10;
const float ADC_res = 5.0/1024.0;

const float R = 10000.0;
const float V = 5.0;
const float P1 = -1.338;
const float P2 = 18.05;

const float threshold = 1000.0;
const float trigger = 200.0;
int led_state = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(pin,OUTPUT);
}

float dig2analog(int digital_value){
  
  float analog_value = ADC_res * digital_value;

  return analog_value;
}

float volt2lux(float volt){
  float LDR;
  float log_lux;

  LDR = R * volt/(V - volt);
  log_lux = P1 * log(LDR) + P2;

  return exp(log_lux);  
}

void compute_weights(float *sensors, float *weights){

  int count = 0;
  int n_zeros = 0;

  for(int i=0; i<n_sensors; i++){
    for(int j=0; j<n_sensors; j++){
      if(abs(sensors[i] - sensors[j]) > threshold){
        count++;
      }
    }
    if(count == n_sensors-1){
      weights[i] = 0.0;
    }
    count = 0;
  }

  for(int k=0; k<n_sensors; k++){
    if(weights[k] == 0.0){
      n_zeros++;
    }
  }

  for(int l=0; l<n_sensors; l++){
    if(weights[l] != 0.0){
      weights[l] = 1.0/(n_sensors - n_zeros);
    }
  }

}

float sensor_fusion(float *sensors, float *weights){

  float sensor_val = 0.0;

  for(int i=0; i<n_sensors; i++){
    sensor_val += weights[i] * sensors[i];
  }

  return sensor_val;
}

void loop() {

  int S1;
  int S2;
  int S3;
  float sensors[3];
  float weights[3] = {1.0/3.0, 1.0/3.0, 1.0/3.0};
  float fusion;
  float time1;
  float time2;
  
  unsigned long curr_time = millis();

  if(curr_time - prev_time >= sample_time){
    //time1 = millis();
    S1 = analogRead(sensor_1);
    S2 = analogRead(sensor_2);
    S3 = analogRead(sensor_3);

    sensors[0] = volt2lux(dig2analog(S1));
    sensors[1] = volt2lux(dig2analog(S2));
    sensors[2] = volt2lux(dig2analog(S3));

    compute_weights(sensors, weights);
    fusion = sensor_fusion(sensors,weights);
    /*time2 = millis()-time1;
    Serial.print("Operations time: ");
    Serial.println(time2);*/
    Serial.print(sensors[0]);
    Serial.print(",");
    Serial.print(sensors[1]);
    Serial.print(",");
    Serial.print(sensors[2]);
    Serial.print(",");
    Serial.println(fusion);

    if(fusion <= trigger && led_state == LOW){
      digitalWrite(pin,HIGH);
      led_state = HIGH;
    }
    if(fusion > trigger && led_state == HIGH){
      digitalWrite(pin,LOW);
      led_state = LOW;
    }
    prev_time = curr_time;
  }
  
}
