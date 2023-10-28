const int E1 = 3; ///<Motor1 Speed
const int M1 = 4; ///<Motor1 Direction
const int trigPin = 7;
const int echoPin = 8;
float temp = 220 ;
float dernier = 220;

template <int order> // order is 1 or 2
class LowPass
{
  private:
    float a[order];
    float b[order+1];
    float omega0;
    float dt;
    bool adapt;
    float tn1 = 0;
    float x[order+1]; // Raw values
    float y[order+1]; // Filtered values

  public:  
    LowPass(float f0, float fs, bool adaptive){
      // f0: cutoff frequency (Hz)
      // fs: sample frequency (Hz)
      // adaptive: boolean flag, if set to 1, the code will automatically set
      // the sample frequency based on the time history.
      
      omega0 = 6.28318530718*f0;
      dt = 1.0/fs;
      adapt = adaptive;
      tn1 = -dt;
      for(int k = 0; k < order+1; k++){
        x[k] = 0;
        y[k] = 0;        
      }
      setCoef();
    }

    void setCoef(){
      if(adapt){
        float t = micros()/1.0e6;
        dt = t - tn1;
        tn1 = t;
      }
      
      float alpha = omega0*dt;
      if(order==1){
        a[0] = -(alpha - 2.0)/(alpha+2.0);
        b[0] = alpha/(alpha+2.0);
        b[1] = alpha/(alpha+2.0);        
      }
      if(order==2){
        float alphaSq = alpha*alpha;
        float beta[] = {1, sqrt(2), 1};
        float D = alphaSq*beta[0] + 2*alpha*beta[1] + 4*beta[2];
        b[0] = alphaSq/D;
        b[1] = 2*b[0];
        b[2] = b[0];
        a[0] = -(2*alphaSq*beta[0] - 8*beta[2])/D;
        a[1] = -(beta[0]*alphaSq - 2*beta[1]*alpha + 4*beta[2])/D;      
      }
    }

    float filt(float xn){
      // Provide me with the current raw value: x
      // I will give you the current filtered value: y
      if(adapt){
        setCoef(); // Update coefficients if necessary      
      }
      y[0] = 0;
      x[0] = xn;
      // Compute the filtered values
      for(int k = 0; k < order; k++){
        y[0] += a[k]*y[k+1] + b[k]*x[k];
      }
      y[0] += b[order]*x[order];

      // Save the historical values
      for(int k = order; k > 0; k--){
        y[k] = y[k-1];
        x[k] = x[k-1];
      }
  
      // Return the filtered value    
      return y[0];
    }
};

float filtreBase1(float rawData, float lastFiltered, int fcoupure, int Te = 0.01)
{
  /*
  Filtre linéaire numérique d'ordre 1, comme on a vu en physique CN2
  */
  float tau = 1/6.28/fcoupure;
  float filtered = (tau-Te)/tau * lastFiltered + Te * rawData;
  return filtered;
}

float passebas(float cm , float temp, float valeurLim){

  float cmFiltre ;

  if ( cm <= valeurLim ) {
    float cmFiltre = cm ;
  }
  else {
    float cmFiltre = temp ;
  }

  return cmFiltre ;
}

void M1_advance(char Speed) ///<Motor1 Advance
{
  digitalWrite(M1,LOW);
  analogWrite(E1,Speed);
}

void M1_back(char Speed) ///<Motor1 Back off
{
 digitalWrite(M1,HIGH);
 analogWrite(E1,Speed);
}

void M1_stop()
{
  digitalWrite(M1,HIGH);
  analogWrite(E1, 0);
}

int mesureSonar()
{
  long duration, cm;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);

  return cm ;
}

void setup() {
  ///Serial.begin(115200);
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  for(int i=3;i<6;i++)
    pinMode(i,OUTPUT);
  
}

void loop() {

  float cm = mesureSonar();
  float valeurLim = cm + 50 ;
  float cmFiltre = passebas(cm , temp, valeurLim);
  float valeurFiltree = filtreBase1(cm, dernier, 15, 0.01);

  // LowPass<2> lp(6000, 100, true);

  // float cmf = lp.filt(cm);

  float vitesse = map(valeurFiltree,5,100,100,0); //modifie

  Serial.print(cm);
  Serial.print("cm  ");
  Serial.print(cmFiltre);
  //Serial.print("cmFiltre ");
  //Serial.print(valeurFiltree);
  Serial.print("valeurFiltree ");
  Serial.println();


  if ( valeurFiltree <= 100) { // modifie
    M1_advance(vitesse);
  }
  else {
    M1_stop();
  }

  float temp = cmFiltre;
  float dernier = valeurFiltree;
  
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

