import 'package:flutter/material.dart';
import 'package:flutter/rendering.dart';
import 'package:flutter_unity_widget_example/database_helper.dart';
import 'package:sqflite/sqflite.dart';

class DataBase extends StatefulWidget {
  @override
  _DataBaseEx createState() => new _DataBaseEx();
}

class _DataBaseEx extends State<DataBase> {
  int entryCount;

  @override
  Widget build(BuildContext context) {

    return Scaffold(
      appBar: AppBar(
        title: Text("Database"),
      ),
      body: Center(
        child: ListView(
          //mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget> [ 
            // FlatButton(onPressed: () async{ // insert function

            //   int i = await DatabaseHelper.instance.insert({
            //     DatabaseHelper.columnName : 'Tom'
            //   });

            //   print('the inserted id is $i');

            // }, child: Text('insert'), color: Colors.grey[400]),
            FlatButton(onPressed: () async{ // query function

              List<Map<String, dynamic>> queryRows;
              List<Map<String, dynamic>> count = await DatabaseHelper.instance.getCount();
              for (int i = 1; i < Sqflite.firstIntValue(count); i ++) {
                queryRows = await DatabaseHelper.instance.queryAll(i);
                print(queryRows[0]['quaterion']);
              }

            }, child: Text('query'), color: Colors.green[400]),
            FlatButton(onPressed: () async{ // delete function

              int rowsAffected = await DatabaseHelper.instance.delete();
              print(rowsAffected);

            }, child: Text('delete'), color: Colors.red[400]),
            FlatButton(onPressed: () async{ // COUNT function

              List<Map<String, dynamic>> count = await DatabaseHelper.instance.getCount();
              print(Sqflite.firstIntValue(count));
              setState((){
                entryCount = Sqflite.firstIntValue(count);
              });

            }, child: Text('count'), color: Colors.red[100]),
            new Text('count: ' + '$entryCount')
          ],
        ),
      ),
    );
  }
}
