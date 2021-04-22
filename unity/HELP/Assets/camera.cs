using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class camera : MonoBehaviour
{
    public Camera cam;

    // Start is called before the first frame update
    float x;
    float y;
    float z; 
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        x = transform.position.x;
        y = transform.position.y;
        z = transform.position.z; 
        if (Input.GetKey("Space"))
        {
            cam.transform.TransformPoint = (x,y,z);
     
        }
    }
}
