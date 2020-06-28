int led1 = 18;
int led2 = 19;

TaskHandle_t Task1, Task2; // Creates a placeholder for several tasks running on different cores.

void createTask1(void * parameter) // Creates the first task 
{
  for(;;)
  {
    Serial.print("This task runs on core ");
    Serial.println(xPortGetCoreID());
    digitalWrite(led1,HIGH);
    delay(1000);
    digitalWrite(led1,LOW);
    delay(1000);
  }
}

void createTask2(void * parameter) // Creates the second task
{
  for(;;)
  {
    Serial.println("This task runs on core ")
    digitalWrite(led2,HIGH);
    delay(1200);
    digitalWrite(led2,LOW);
    delay(1200);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);

  xTaskCreatePinnedToCore(  // Defines the task, assigns priority and assigns the core on which the task runs.
    createTask1, //  name of the function in which the task is stored
    "Task1",     // name of the task
    1000,        // Size of a task stack
    NULL,        // Pointer that will be used as the parameter for the task
    1,           // Priority of the task
    &Task1,      // Taskhandle/placeholder of the task
    1);          // Core on which the task should run
  
}

void loop() {
  
}
