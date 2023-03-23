using Interop;
using Microsoft.AspNetCore.Mvc;
using SkiaSharp;
using System.Diagnostics;
using System.IO;
using System.Runtime.Intrinsics.Arm;
using TarkovInventoryAssistant_Server.Models;
using TarkovInventoryAssistant_Server.source.Models;
using TarkovInventoryAssistant_Server.Utilities;

namespace TarkovInventoryAssistant_Server.Controllers
{
    public class TIAController : Controller
    {



        public TIAController(IWebHostEnvironment env, CoreInterop core)
        {
            m_core = core;
            m_env = env;
        }

        // Show the selected image from the active catalog.
        public IActionResult ShowCatalogImage(string filePath, string fileName, string contentType)
        {
            var fileContentResult = new FileContentResult(System.IO.File.ReadAllBytes(filePath), contentType)
            {
                FileDownloadName = fileName
            };

            return fileContentResult;
        }

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Privacy()
        {
            return View();
        }


        [HttpPost]
        public IActionResult AddImage(IFormFile image)
        {
            if (image != null && image.Length > 0)
            {
                using (var memoryStream = new MemoryStream())
                {
                    image.CopyTo(memoryStream);
                    byte[] bytes = memoryStream.ToArray();// Change "image/jpeg" to the appropriate content type for your image
                    return new FileContentResult(bytes, "image/png"); 
                }
            }
            return Content("<p>No image was uploaded.</p>");
        }


        [HttpPost]
        public IActionResult DisplayResults(string ImageString)
        {
            if (ImageString == null)
                return BadRequest(new { error = "Invalid input" });
            byte[] imageBytes = Convert.FromBase64String(ImageString);

            // Create a MemoryStream from the byte array
            using MemoryStream ms = new MemoryStream(imageBytes);

            // Create an SKBitmap object from the MemoryStream
            using SKBitmap image = SKBitmap.Decode(ms);

            DetectionResultMarshal[] detMat = m_core.detectImageContent(image);

            bool collapseSimilar = true; // TODO: this needs to be an option in the View.

            // TODO: Make this line better, and protect against crash when string is empty.
            string relativeCatalogPath = "/" + Path.GetRelativePath(m_env.WebRootPath, m_core.getACTIVE_CATALOG()).Replace("\\","/");
            // END TODO

            ResultsListModel rlmodel = new ResultsListModel();


            if (collapseSimilar)
            {
                // name, <detMatIndex, count> detMatIndex is index of the first item of the given name.
                Dictionary<string, Utilities.Pair<int, int>> uniqueDetectionMap = new Dictionary<string, Utilities.Pair<int, int>>();

                for (int i = 0; i < detMat.Length; i++)
                {
                    DetectionResultMarshal drm = detMat[i];
                    rlmodel.totalvaluecounter.addResultEntry(drm);
                    if (uniqueDetectionMap.ContainsKey(drm.name))
                    {
                        uniqueDetectionMap[drm.name].Second++;
                    }
                    else
                    {
                        uniqueDetectionMap[drm.name] = new Utilities.Pair<int, int>(i, 1);
                    }
                }
                foreach (string name in uniqueDetectionMap.Keys)
                {
                    rlmodel.results.Add(new DetectionResultsModel(detMat[uniqueDetectionMap[name].First], relativeCatalogPath, uniqueDetectionMap[name].Second));
                }

            }
            else
            {
                // Simply add every DetectionResult
                for (int i = 0; i < detMat.Length; i++)
                {
                    rlmodel.totalvaluecounter.addResultEntry(detMat[i]);
                    rlmodel.results.Add(new DetectionResultsModel(detMat[i], relativeCatalogPath));
                }
            }

            return PartialView("Result", rlmodel);
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }


        private readonly CoreInterop m_core;
        private readonly IWebHostEnvironment m_env;
    }
}