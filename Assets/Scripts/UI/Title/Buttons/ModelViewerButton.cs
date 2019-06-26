using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace BVE.UI.Title.Buttons {
    public class ModelViewerButton : MonoBehaviour {
        public void Click() {
            SceneManager.LoadScene("Model Viewer");
        }
    }
}
