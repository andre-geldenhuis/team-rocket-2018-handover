#include "sensor.h"

void Sensor::record(void)
{
	// debugger.printf("record - len: %d - headnull? %d - tailnull? %d\r\n", list_len, head==NULL, tail==NULL);
	// Reads from GPS
	Sensor_Data* data = read();

	if (data == NULL)
	{
		return;
	}

	// If list is empty, start a new one else appends to existing
	if (head == NULL) {
		tail = head = data;
	} else {
		tail->next = data;
		tail = data;
	}
	list_len++;
	// debugger.println("finished_record");
}

void Sensor::clear_list(void)
{
	// debugger.println("clear_list");
	if (head == NULL) 
	{
		return;
	}
	Sensor_Data* curr = head;
	Sensor_Data* curr_next = head->next;
	head = NULL;
	tail = NULL;
	// Itteravly clears memory used by data objects
	while(curr_next != NULL) {
		delete curr;
		curr = curr_next;
		curr_next = curr->next;
	}
	delete curr;
	// delete curr_next;
	list_len = 0;
}

String Sensor::list_to_string(void)
{
	// debugger.println("list_to_string");
	if (head == NULL)
	{
		return "";
	}
	String message = "";
	Sensor_Data* data = head;

	message += data->to_string();
	data = data->next;

	while (data != NULL)
	{
		message += "\r\n" + data->to_string();
		data = data->next;
	}

	return message;
}

Sensor_Data* Sensor::get_list(void)
{
	return head;
}

Sensor_Data* Sensor::get_tail(void)
{
	return tail;
}

size_t Sensor::get_length(void)
{
	return list_len;
}