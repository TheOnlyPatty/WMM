// Copyright 2017, Paul DeMarco.
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

import 'dart:async';
import 'dart:math';

import 'package:flutter_unity_widget/flutter_unity_widget.dart';
import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'widgets.dart';
import 'screens/menu_screen.dart';
import 'package:flutter_unity_widget_example/database_helper.dart';
import 'package:sqflite/sqflite.dart';

class UnityScreen extends StatelessWidget {

  UnityScreen({Key key}) : super(key: key);

  static final GlobalKey<ScaffoldState> _scaffoldKey =
  GlobalKey<ScaffoldState>();
  UnityWidgetController _unityWidgetController;
  double _sliderValue = 0.0;
  int x = 0;


  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Safe Mode Screen'),

      ),
      body: Card(
        margin: const EdgeInsets.all(8),
        clipBehavior: Clip.antiAlias,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(20.0),
        ),
        child: Stack(
          children: <Widget>[
            UnityWidget(
              onUnityViewCreated: onUnityCreated,
              isARScene: true,
              safeMode: true,
              onUnityMessage: onUnityMessage,
            ),
            Positioned(
              bottom: 20,
              left: 20,
              right: 20,
              child: Card(
                elevation: 10,
                child: Column(
                  children: <Widget>[

                    RaisedButton(
                      onPressed: () async{
            
                        List<Map<String, dynamic>> queryRows;
                        List<Map<String, dynamic>> count = await DatabaseHelper.instance.getCount();
                        String value;
                        for (int i = 1; i < Sqflite.firstIntValue(count); i ++) {
                          queryRows = await DatabaseHelper.instance.queryAll(i);
                          value = queryRows[0]['quaterion'];
                          print(i);
                          await Future.delayed(const Duration(milliseconds: 30));
                          setOrientation(value);
                        }

                      },
                      child: const Text('Press to start demo!', style: TextStyle(fontSize: 20)),
                    ),


                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
  void setOrientation(String quat){
    _unityWidgetController.postMessage(
      'Cube',
      'SetOrientation',
      quat,
    );
  }
  void setRotationSpeed(String speed) {
    _unityWidgetController.postMessage(
      'Cube',
      'SetRotationSpeed',
      speed,
    );
  }

  void onUnityMessage(controller, message) {
    print('Received message from unity: ${message.toString()}');
  }

  // Callback that connects the created controller to the unity controller
  void onUnityCreated(controller) {
    this._unityWidgetController = controller;
  }
}