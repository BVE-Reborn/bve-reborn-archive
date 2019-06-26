using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace BVE.UI.Title {
    public class VersionNumber : MonoBehaviour {
        // Start is called before the first frame update
        void Start() {
            var textComponent = gameObject.GetComponent<TMPro.TextMeshProUGUI>();
            textComponent.SetText($"{Application.version} on Unity v{Application.unityVersion}");
        }

        // Update is called once per frame
        void Update() { }
    }
}