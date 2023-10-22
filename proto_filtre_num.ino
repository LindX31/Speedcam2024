void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


float filtreBase(float rawData, float lastFiltered, int fcoupure, int Te = 0.01)
{
    float tau = 1/fcoupure;
    float filtered = (tau-Te)/tau * lastFiltered + Te * rawData;
    return filtered;
}

float filtreTableau(float rawData)