function enableScript(newScriptObject) {
   newScriptObject.set_r(100);
   newScriptObject.set_g(111);
   newScriptObject.set_b(255);
   newScriptObject.set_length(80);
   newScriptObject.set_width(30);
   print('The color and size of the character has been changed! \n');
   print(' new r / new g / new b / new length / new width ');
   return newScriptObject.get_r() + ',' + newScriptObject.get_g() + ',' + newScriptObject.get_b() + ',' + newScriptObject.get_length() + ',' + newScriptObject.get_width();
}