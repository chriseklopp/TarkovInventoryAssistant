using Microsoft.AspNetCore.Mvc;
using TarkovInventoryAssistant_Server.Models;

namespace TarkovInventoryAssistant_Server.Controllers
{
    public class ResultController : Controller
    {
        private readonly ILogger<ResultController> _logger;

        public ResultController(ILogger<ResultController> logger)
        {
            _logger = logger;
        }


        public IActionResult DisplayResults()
        {
            return View();
        }


        private List<DetectionResultsModel> results = new List<DetectionResultsModel>();
    }
}