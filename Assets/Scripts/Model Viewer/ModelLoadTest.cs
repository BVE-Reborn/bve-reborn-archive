using System;
using System.Data;
using System.IO;
using Native.bve.core.image;
using Native.bve.parsers.b3d_csv_object;
using UnityEngine;
using UnityEngine.Assertions;
using UnityEngine.Experimental.Rendering;
using Debug = UnityEngine.Debug;
using File = System.IO.File;
using Mesh = UnityEngine.Mesh;

namespace BVE.ModelLoader {
    public class ModelLoadTest : MonoBehaviour {
        public Shader Shader;

        private void LoadModel(string path) {
            if (!File.Exists(path)) {
                throw new ArgumentException($"Path {path} not found");
            }

            var ext = Path.GetExtension(path);
            if (ext != ".b3d" && ext != ".csv") {
                throw new ArgumentException($"File {path} is not a model file");
            }

            string contents;
            using (var file = File.OpenText(path)) {
                contents = file.ReadToEnd();
            }

            using (var obj = ParseObject(ext, contents)) {
                ProcessModel(path, obj);
            }
        }

        private ParsedB3DCSVObject ParseObject(string ext, string contents) {
            switch (ext) {
                case ".b3d":
                    return Native.bve_cs.parse_model_b3d(contents);
                case ".csv":
                    return Native.bve_cs.parse_model_csv(contents);
                default:
                    throw new ArgumentException($"Unknown Extenstion {ext}", nameof(ext));
            }
        }

        private void ProcessModel(string path, ParsedB3DCSVObject obj) {
            var errors = obj.errors;
            for (var i = 0; i < errors.Count; i++) {
                var error = errors[i];
                Debug.LogError($"Error in parsing {path}:{error.line} {error.error}");
            }

            var objMeshCount = obj.meshes.Count;
            for (int meshIndex = 0; meshIndex < objMeshCount; meshIndex++) {
                var gameObject = new GameObject($"test_model{meshIndex}");
                var meshFilter = gameObject.AddComponent<MeshFilter>();
                var meshRenderer = gameObject.AddComponent<MeshRenderer>();

                var mesh = new Mesh();

                var objMesh = obj.meshes[meshIndex];
                var objVerts = objMesh.verts;
                var objIndices = objMesh.indices;

                var vertCount = objVerts.Count;
                var meshVerts = new Vector3[vertCount];
                var meshNormals = new Vector3[vertCount];
                var meshUV = new Vector2[vertCount];

                for (var i = 0; i < vertCount; i++)
                {
                    var vertData = objVerts[i];
                    meshVerts[i] = vertData.position.toVector3();
                    meshNormals[i] = vertData.normal.toVector3();
                    meshUV[i] = vertData.texture_coord.toVector2();
                }

                mesh.vertices = meshVerts;
                mesh.normals = meshNormals;
                mesh.uv = meshUV;

                var indicesCount = objIndices.Count;
                var meshIndices = new int[indicesCount];

                for (var i = 0; i < indicesCount; i++)
                {
                    meshIndices[i] = (int)objIndices[i];
                }

                mesh.triangles = meshIndices;

//                mesh.RecalculateNormals();
                mesh.RecalculateTangents();
                mesh.RecalculateBounds();
                mesh.Optimize();

                meshFilter.mesh = mesh;

                var mat = new Material(Shader);
                var texFullPath = ResolveTextureName(path, objMesh.texture.file);
                var tex = loadTexture(texFullPath);

                mat.mainTexture = tex;

                meshRenderer.material = mat;
            }

            Debug.Log($"Loaded object {path}");


        }

        private string ResolveTextureName(string objPath, string texturePath) {
            if (Path.IsPathRooted(texturePath)) {
                return texturePath;
            }

            var objRelative = Path.Combine(objPath, "..", texturePath);
            if (File.Exists(objRelative)) {
                return Path.GetFullPath(objRelative);
            }

            string _dataDir = Native.bve_cs.core_filesystem_data_directory().path();
            var dataRelative = Path.Combine(_dataDir, "LegacyContent/Railway/Object", texturePath);
            if (File.Exists(dataRelative)) {
                return Path.GetFullPath(dataRelative);
            }

            throw new ArgumentException($"Unknown texture {texturePath}. Tried: \"{objRelative}\" and \"{dataRelative}\"");
        }

        private Texture2D loadTexture(string file) {
            if (!File.Exists(file)) {
                throw new ArgumentException($"Unknown texture {file}");
            }

            using (var loader = new Loader(file)) {
                var dims = loader.dimensions();
                if (loader.valid() == false) {
                    throw new ArgumentException($"Couldn't load image {file}");
                }

                var tex = new Texture2D(dims.x, dims.y, DefaultFormat.LDR, TextureCreationFlags.MipChain);
                var data = loader.data();
                tex.SetPixels(data);
                tex.Apply();
                return tex;
            }
        }

        public void Start() {
            LoadModel(
                "E:\\16) OpenBVE Files\\LegacyContent\\Railway\\Object\\IND Valley Stream1\\cars\\R46_A.b3d");
        }

        // Update is called once per frame
        public void Update() { }
    }
}