using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PickDrop : MonoBehaviour
{
    [SerializeField] private GameObject _target = null;
    [SerializeField] private bool _holding = false;
    public bool jumpToObj;
    private bool jump;
    public int radius;

    // Use this for initialization
    private void Start()
    {
    }

    private void Update()
    {
        GetGoodRayCasting();
        if (Input.GetKey(KeyCode.R)) SceneManager.LoadScene(SceneManager.GetActiveScene().name);

        if (!_holding) return;
        _target.transform.position = transform.position+new Vector3(0,0.4f,2);
        //_target.transform.rotation = transform.rotation;
        if (jump)
        {
            this.GetComponentInParent<Rigidbody>().MovePosition(_target.transform.position - new Vector3(0, 0.4f, 0));
            jump = false;
        }
        _target.transform.position = transform.position+new Vector3(-1,1,1);
        _target.transform.rotation = transform.rotation;

        if (Input.GetKeyUp(KeyCode.F)) Hold(false);
    }

    private void OnTriggerStay(Collider collider)
    {
         
    }

    private void Hold(bool hold = true)
    {
        _holding = hold;
        _target.GetComponent<Rigidbody> ().isKinematic = hold;
        if (!hold) _target = null;
    }
    private void GetGoodRayCasting()
    {
        if (Input.GetMouseButtonDown(0))
        {
            RaycastHit hit;
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            if (Physics.Raycast(ray, out hit, 100000.0f))
            {
                //suppose i have two objects here named obj1 and obj2.. how do i select obj1 to be transformed 
                if (hit.collider.gameObject.tag=="Crate")
                {
                    if (Vector3.Distance(hit.collider.transform.position, transform.position) <= radius)
                    {
                        Debug.Log("clicked on crate");
                        jump = jumpToObj;
                        _target = hit.collider.gameObject;
                        Hold();
                    }
                }
            }
        }
    }
}
