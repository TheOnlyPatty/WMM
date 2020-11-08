import 'dart:io';
import 'package:path_provider/path_provider.dart';
import 'package:sqflite/sqflite.dart';
import 'package:path/path.dart';

class DatabaseHelper{

  static final _dbName = 'myDatabase.db';
  static final _dbVersion = 1;
  static final _tableName = 'IMUdata';

  static final columnID = '_id';
  static final columnName = 'quaterion';

  // making it a singleton class
  DatabaseHelper._privateConstructor();
  static final DatabaseHelper instance = DatabaseHelper._privateConstructor();

  static Database _database;
  Future<Database> get database async{
    if (_database != null) return _database;

    _database = await _initiateDatabase();
    return _database;
  }


  _initiateDatabase () async {
    Directory directory = await getApplicationDocumentsDirectory();
    String path = join(directory.path,_dbName);
    return await openDatabase(path, version: _dbVersion, onCreate: _onCreate);
  }

  Future _onCreate(Database db, int version){
    db.execute(
      '''
      CREATE TABLE $_tableName (
      $columnID INTEGER PRIMARY KEY,
      $columnName TEXT NOT NULL )
      '''
    );
  }

  Future<List<Map<String, dynamic>>> getCount() async {
    Database db = await instance.database;
    return await db.rawQuery('SELECT COUNT(*) FROM $_tableName');
  }

  Future<int> insert(Map<String, dynamic> row) async {
    Database db = await instance.database;
    return await db.insert(_tableName, row);
  }

  Future<List<Map<String, dynamic>>> queryAll(int id) async {
    Database db = await instance.database;
    return await db.query(_tableName, columns: ['quaterion'],where: '$columnID = ?', whereArgs: [id]);
    //return await db.query(_tableName);
  }

  Future update(Map<String, dynamic> row) async {
    Database db = await instance.database;
    int id = row[columnID];
    return await db.update(_tableName, row, where:'$columnID = ?', whereArgs: [id]);

  }

  Future<int> delete() async{
    Database db = await instance.database;
    return await db.delete(_tableName);
    //return await db.delete(_tableName, where: '$columnID = ?', whereArgs: [id]);
  }


}