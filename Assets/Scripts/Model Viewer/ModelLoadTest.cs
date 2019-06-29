using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using Native.bve.core.image;
using Native.bve.parsers.b3d_csv_object;
using UnityEngine;
using UnityEngine.Experimental.Rendering;
using UnityEngine.Networking;
using UnityEngine.Windows;
using Debug = UnityEngine.Debug;

namespace BVE.ModelLoader {
    public class ModelLoadTest : MonoBehaviour {
        private const string Test = @"
;;;;;;;
CreateMeshBuilder,  ;;Pole

Cylinder, 16, 0.06, 0.06, 2.2,
Translate, 0, 0.9, -0.198,

SetColor, 11,11,11,

CreateMeshBuilder,  ;;Pole2

Cylinder, 16, 0.1, 0.1, 1.2,
Translate, 0, 0.40, -0.198,

SetColor, 11,11,11,
";

        // Start is called before the first frame update
        public void Start() {
            var sw = Stopwatch.StartNew();
            var parsed = Native.bve_cs.parse_csv(Test);
            sw.Stop();
            Debug.Log(sw.ElapsedMilliseconds);
            Debug.Log(sw.ElapsedTicks);
            foreach (var parsedMesh in parsed.meshes) {
                foreach (var vert in parsedMesh.verts) {
                    var position = vert.position.toVector3();
//                    Debug.Log(position);
                }


            }
//            string filename = "E:\\16) OpenBVE Files\\LegacyContent\\Railway\\Object\\2ndAveT\\Ballast.BMP";
            string filename = "E:\\16) OpenBVE Files\\LegacyContent\\Railway\\Object\\8thAve\\3yon.png";
            var loader = new Loader(filename);
            var dims = loader.dimensions();
            if (loader.valid() == false) {
                Debug.Log("Couldn't load image.");
            }

            var tex = new Texture2D(dims.x, dims.y, TextureFormat.RGBA32, true);
            var data = loader.data();
            tex.SetPixels(data);
            tex.Apply();
            var mesh = GetComponent<MeshRenderer>();
            mesh.material.SetTexture("_MainTex", tex);
        }

        // Update is called once per frame
        public void Update() { }
    }
}