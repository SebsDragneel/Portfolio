using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;

public class LoadScene : MonoBehaviour {

    public float SceneToLoad = 0;
    public bool Load = false;

    void Update() {
        if(Load) {
            SceneManager.LoadScene((int) SceneToLoad);
        }
    }

    public void ChangeScene(string mSceneName)
    {
        SceneManager.LoadScene(mSceneName);
    }

    public void ChangeScene(int mSceneID) {
        SceneManager.LoadScene(mSceneID);
    }
	
}
