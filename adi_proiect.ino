#include <SoftwareSerial.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;


SemaphoreHandle_t xBinarySemaphore;
SoftwareSerial portOne(10, 11);

void setup() {
  Serial.begin(9600);
  portOne.begin(9600);

  pinMode(3, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(11, OUTPUT);

  xBinarySemaphore = xSemaphoreCreateBinary();
  xTaskCreate(TaskLED1,  "Led", 128, NULL, 1, &TaskHandle_1 );
  xTaskCreate(TaskSerial,  "Serial", 128, NULL, 1, NULL);
  xTaskCreate(TaskLED2,  "Led", 128, NULL, 1, &TaskHandle_2);
  xTaskCreate(TaskLED3,  "Led", 128, NULL, 1, &TaskHandle_3);
  xSemaphoreGive(xBinarySemaphore);
}

void TaskLED1(void *pvParameters)
{
  while (1)
  {
    digitalWrite(3, HIGH);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    digitalWrite(3, LOW);
    vTaskDelay(10 / portTICK_PERIOD_MS);

    vTaskSuspend(TaskHandle_1);
  }
}

void TaskLED2(void *pvParameters)
{
  while (1)
  {
    digitalWrite(7, HIGH);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    digitalWrite(7, LOW);
    vTaskDelay(5 / portTICK_PERIOD_MS);

    vTaskSuspend(TaskHandle_2);
  }
}

void TaskLED3(void *pvParameters)
{
  while (1)
  {
    digitalWrite(11, HIGH);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    digitalWrite(11, LOW);
    vTaskDelay(10 / portTICK_PERIOD_MS);

    vTaskSuspend(TaskHandle_3);
  }
}

void TaskSerial(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  while (1) {
    portOne.listen();
    while (portOne.available() > 0) {
      int inByte = portOne.parseInt();
      xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
      if (inByte == 1) {
        Serial.println("Task 1");
        vTaskResume(TaskHandle_1);
      }
      else if (inByte == 2)
      {
        Serial.println("Task 2");
        vTaskResume(TaskHandle_2);
      }
      else if (inByte == 3)
      {
        Serial.println("Task 3");
        vTaskResume(TaskHandle_3);
      }
      if (inByte == 4)
      {
        Serial.println("Task 4");
      }
      else {
        digitalWrite(15, LOW);
      }
      xSemaphoreGive(xBinarySemaphore);
    }
  }
}
void loop() {
}
