#include "scriptObject.h"

scriptObject::scriptObject() {}

int scriptObject::GetR() {
	return m_r;
}
int scriptObject::GetG() {
	return m_g;
}
int scriptObject::GetB() {
	return m_b;
}
float scriptObject::GetLenght() {
	return m_length;
}
float scriptObject::GetWidth() {
	return m_width;
}
void scriptObject::SetR(int val) {
	this->m_r = val;
}
void scriptObject::SetG(int val) {
	this->m_g = val;
}
void scriptObject::SetB(int val) {
	this->m_b = val;
}
void scriptObject::SetLength(float val) {
	this->m_length = val;
}
void scriptObject::SetWidth(float val) {
	this->m_width = val;
}
