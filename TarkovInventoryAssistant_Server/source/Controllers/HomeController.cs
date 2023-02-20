using Interop;
using Microsoft.AspNetCore.Mvc;
using System.Diagnostics;
using TarkovInventoryAssistant_Server.Models;

namespace TarkovInventoryAssistant_Server.Controllers
{
    public class HomeController : Controller
    {
        private readonly CoreInterop m_core;

        public HomeController(CoreInterop core)
        {
            m_core = core;
        }

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Privacy()
        {
            return View();
        }

        public IActionResult Result(List<DetectionResultsModel> detections)
        {
            return View(detections);
        }

        public IActionResult SubmitImage(ImageModel imageModel)
        {
            DetectionResultMarshal[] detMat = m_core.detectImageContent(69);
            List<DetectionResultsModel> results = new List<DetectionResultsModel>(detMat.Length);
            for(int i = 0; i < detMat.Length; i++)
            {
                results.Add(new DetectionResultsModel(detMat[i]));
            }
            return View("Result", results);
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }



    }
}